/*
 * A097560  a(n) = number of terms among {a(1), a(2), a(3), ..., a(n-1)} that
 *          are coprime to n; a(1)=3.
 * https://oeis.org/A097560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097560 – a(n) = number of previous terms coprime to n, a(1)=3.
   The implementation caches computed terms for fast repeated access. */
#include <stdlib.h>
#include "../utils/gcd.h"   /* provides seq_t and idx_t definitions */

/* A local Euclidean gcd for seq_t (seq_t may be up to 128‑bit). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms.  cache[0] is unused, a(1) stored at cache[1]. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t len   = 0;   /* largest index stored */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap = 2;                          /* need space for indices 0 and 1 */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* OOM → 0 (rare case) */
        cache[0] = 0;
        cache[1] = (seq_t)3;               /* a(1) = 3 */
        len = 1;
    }

    if (n == (idx_t)0) return (seq_t)0;     /* behaviour for n<1 is arbitrary */

    /* Grow cache to accommodate n. */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;    /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;    /* OOM → 0 */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n. */
    if (n > len) {
        for (idx_t k = len + 1; k <= n; ++k) {
            seq_t count = 0;
            seq_t nk = (seq_t)k;            /* n as seq_t for gcd */
            for (idx_t j = 1; j < k; ++j) {
                if (gcd_seq_t(cache[j], nk) == (seq_t)1)
                    ++count;
            }
            cache[k] = count;
        }
        len = n;
    }
    return cache[n];
}
