/*
 * A162684  a(1)=1. a(n) = the number of terms among (a(1),a(2),...a(n-1))
 *          that either are coprime to n or divide n.
 * https://oeis.org/A162684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k < n : gcd(a(k), n) = 1 or a(k) | n}|
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* array of a(1)..a(max_n) */
static idx_t  max_n = 0;      /* highest n computed */
static size_t cap   = 0;      /* allocated size of cache */

/* Ensure the cache holds at least 'needed' elements. */
static void ensure_cap(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but harmless */

    /* Initialise the cache for n = 1, if not already done. */
    if (max_n == 0) {
        ensure_cap(1);
        cache[0] = 1;          /* a(1) = 1 */
        max_n = 1;
    }

    /* Grow the cache up to n, if necessary. */
    if (n > max_n) {
        ensure_cap((size_t)n);
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            seq_t cur_s = (seq_t)cur;   /* cast to seq_t for arithmetic */
            seq_t cnt   = 0;            /* count of qualifying previous terms */

            /* Scan all earlier terms a(1)..a(cur-1). */
            for (idx_t k = 0; k < cur - 1; ++k) {
                seq_t ak = cache[k];
                if ((cur_s % ak) == 0 || gcd(ak, cur_s) == 1) ++cnt;
            }
            cache[cur - 1] = cnt;
        }
        max_n = n;
    }

    return cache[n - 1];
}
