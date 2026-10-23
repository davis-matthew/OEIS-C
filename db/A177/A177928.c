/*
 * A177928  Let n be the number whose square n^2 has the decimal expansion {
 *          d(1) d(2) ... d(D) }, and let q be the corresponding number whose
 *          decimal expansion is { d(2) d(3) ... d(D) d(1)}. Sequence lists
 *          numbers n dividing q.
 * https://oeis.org/A177928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS sequence A177928.
 * For each integer n (with 1‑based index), the function tests whether
 * n divides the cyclic shift of its square n².  The result is returned
 * at the given index n.  The implementation keeps a static cache so
 * that repeated calls are efficient.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>   /* for uint64_t */

static seq_t *cache = NULL;        /* cached sequence values */
static size_t  cache_len = 0;      /* number of cached terms   */
static size_t  cache_cap = 0;      /* allocated capacity       */
static idx_t   next_test = 1;      /* next integer to test    */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for index 0 */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t k = next_test;            /* candidate integer */

        /* Use unsigned __int128 for intermediate arithmetic
           to avoid overflow for all supported seq_t sizes.
           (For seq_t smaller than 64 bits the modulo will
            automatically reduce the result.) */
        unsigned __int128 sq = (unsigned __int128)k * k;

        /* Count decimal digits of sq */
        unsigned __int128 tmp = sq;
        unsigned int digits = 0;
        do { ++digits; tmp /= 10; } while (tmp > 0);

        /* 10^(digits‑1) */
        unsigned __int128 pow10d = 1;
        for (unsigned int i = 1; i < digits; ++i) pow10d *= 10;

        /* Split sq into most significant digit and remainder */
        unsigned __int128 msd = sq / pow10d;
        unsigned __int128 rem = sq % pow10d;

        /* Form the cyclically shifted number q */
        unsigned __int128 q = rem * 10 + msd;

        /* Check divisibility */
        if (q % k == 0) {
            /* Ensure sufficient capacity for the cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_cache) exit(1);   /* out of memory */
                cache = tmp_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;   /* store result */
        }
        ++next_test;
    }

    return cache[n - 1];   /* return the n‑th term (1‑based) */
}
