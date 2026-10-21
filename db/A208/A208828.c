/*
 * A208828  Number of 7-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 8*a(n-3) + 14*a(n-4) - 14*a(n-6) +
 *            8*a(n-7) + 3*a(n-8) - 4*a(n-9) + a(n-10). Initial values (n = 1
 *            … 10): 32, 609, 4390, 19657, 65134, 177097, 417204, 883409,
 *            1720628, 3135633
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* start the cache with the first 10 terms when needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        cache[0] = 32;      /* a(1) */
        cache[1] = 609;     /* a(2) */
        cache[2] = 4390;    /* a(3) */
        cache[3] = 19657;   /* a(4) */
        cache[4] = 65134;   /* a(5) */
        cache[5] = 177097;  /* a(6) */
        cache[6] = 417204;  /* a(7) */
        cache[7] = 883409;  /* a(8) */
        cache[8] = 1720628; /* a(9) */
        cache[9] = 3135633; /* a(10) */
        cache_len = 10;
    }

    /* if the index is already in the cache, return it */
    if (n <= (idx_t)cache_len) {
        return cache[(size_t)(n - 1)];
    }

    /* otherwise extend the cache up to n */
    while (cache_len < (size_t)n) {
        /* compute a(next) using the recurrence */
        size_t i = cache_len;          /* this will be the index of the new term (0‑based) */
        seq_t a1 = cache[i - 1];       /* a(n-1)    */
        seq_t a2 = cache[i - 2];       /* a(n-2)    */
        seq_t a3 = cache[i - 3];       /* a(n-3)    */
        seq_t a4 = cache[i - 4];       /* a(n-4)    */
        seq_t a6 = cache[i - 6];       /* a(n-6)    */
        seq_t a7 = cache[i - 7];       /* a(n-7)    */
        seq_t a8 = cache[i - 8];       /* a(n-8)    */
        seq_t a9 = cache[i - 9];       /* a(n-9)    */
        seq_t a10 = cache[i - 10];     /* a(n-10)   */

        seq_t next = 4 * a1
                    - 3 * a2
                    - 8 * a3
                    + 14 * a4
                    - 14 * a6
                    + 8 * a7
                    + 3 * a8
                    - 4 * a9
                    + a10;

        /* grow the cache array */
        seq_t *new_cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort gracefully */
            /* In this limited environment, we simply return 0 */
            return 0;
        }
        cache = new_cache;
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)(n - 1)];
}
