/*
 * A079792  a(1) = 1, a(n) = a(n-1)/gcd(a(n-1),n) if gcd(a(n-1),n) > 1
 *          otherwise a(n) is the concatenation of a(n-1) and n.
 * https://oeis.org/A079792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; for n>1, let g = gcd(a(n‑1), n). If g > 1, a(n) =
 *            a(n‑1) / g; otherwise a(n) = concatenate decimal(a(n‑1), n)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

static seq_t *cache = NULL;         /* cached values a(1), a(2), …  */
static size_t  cache_cap = 0;       /* allocated capacity in seq_t units */
static size_t  cache_len = 0;       /* number of terms stored (cache_len ≥ 1 after init) */

static unsigned digits_of_uint64(idx_t x)
{
    unsigned d = 0;
    if (x == 0) return 1;
    for (unsigned long long t = x; t; t /= 10) d++;
    return d;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    /* lazy init: cache[0] = a(1) = 1 */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();           /* out of memory */
        cache[0] = (seq_t)1;
        cache_len = 1;                 /* we now have a(1) */
    }

    /* grow cache if needed */
    while (cache_len < n) {
        idx_t idx = (idx_t)(cache_len + 1);   /* 1‑based index of term to compute */
        seq_t prev = cache[cache_len - 1];    /* a(idx‑1) */
        seq_t g = gcd(prev, (seq_t)idx);

        seq_t next;
        if (g > 1) {
            next = prev / g;                /* division case */
        } else {
            unsigned d = digits_of_uint64(idx);
            seq_t pow10 = 1;
            for (unsigned i = 0; i < d; ++i) pow10 *= 10;   /* 10^d as seq_t */
            next = prev * pow10 + (seq_t)idx;               /* concatenation */
        }

        /* ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) abort();
            cache = new_ptr;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
