/*
 * A167900  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*Sum_{j=1..15} a(n-j) - 28*a(n-16) (n>16) a(0) = 1 a(1)
 *            = 9 a(2) = 72 a(3) = 576 a(4) = 4608 a(5) = 36864 a(6) = 294912
 *            a(7) = 2359296 a(8) = 18874368 a(9) = 150994944 a(10) =
 *            1207959552 a(11) = 9663676416 a(12) = 77309411328 a(13) =
 *            618475290624 a(14) = 4947802324992 a(15) = 395824185999...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Initial values a(0)...a(15) */
    static const seq_t init[16] = {
        1ULL, 9ULL, 72ULL, 576ULL, 4608ULL,
        36864ULL, 294912ULL, 2359296ULL, 18874368ULL,
        150994944ULL, 1207959552ULL, 9663676416ULL,
        77309411328ULL, 618475290624ULL,
        4947802324992ULL, 39582418599936ULL
    };

    /* Allocate and initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 64;                          /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init[i];
        cache_len = 16;
    }

    /* Return immediately if value already cached */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow cache to accommodate n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute required terms using the recurrence */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        seq_t sum = 0;
        for (idx_t offset = 1; offset <= 15; ++offset)
            sum += cache[k - offset];

        if (k == 16) {
            /* The non‑homogeneous case (n = 16) */
            cache[k] = 7 * sum - 28 * cache[0] + 1;
        } else {
            cache[k] = 7 * sum - 28 * cache[k - 16];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
