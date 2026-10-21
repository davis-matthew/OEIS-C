/*
 * A217640  Number of nX3 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random 0..2 nX3 array.
 * https://oeis.org/A217640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 14‑term linear recurrence * a(n) = 5*a(n-1) +
 *            3*a(n-2) + 9*a(n-3) -10*a(n-4) - 7*a(n-5) * -166*a(n-6)
 *            -3*a(n-7) -122*a(n-8) -71*a(n-9) +165*a(n-10) * +43*a(n-11)
 *            -43*a(n-12) -7*a(n-13) +3*a(n-14) for n>15 * with initial values
 *            * a(1)=9, a(2)=27, a(3)=163, a(4)=877, a(5)=5119, a...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial seed values for indices 1..15 (index 0 unused). */
    static const seq_t init[16] = {
        (seq_t)0ULL,                     /* 0 : unused */
        (seq_t)9ULL,                     /* a(1) */
        (seq_t)27ULL,                    /* a(2) */
        (seq_t)163ULL,                   /* a(3) */
        (seq_t)877ULL,                   /* a(4) */
        (seq_t)5119ULL,                  /* a(5) */
        (seq_t)29101ULL,                 /* a(6) */
        (seq_t)166193ULL,                /* a(7) */
        (seq_t)949883ULL,                /* a(8) */
        (seq_t)5424617ULL,               /* a(9) */
        (seq_t)30992313ULL,              /* a(10) */
        (seq_t)177045523ULL,             /* a(11) */
        (seq_t)1011403721ULL,            /* a(12) */
        (seq_t)5777856321ULL,            /* a(13) */
        (seq_t)33007064639ULL,           /* a(14) */
        (seq_t)188559321601ULL           /* a(15) */
    };

    /* Static memoisation cache. */
    static seq_t *cache = NULL;      /* array of cached values */
    static idx_t cache_len = 0;      /* largest n that is cached (>=0) */
    static size_t cache_cap = 0;     /* allocated capacity (in elements) */

    /* Ensure that the cache is large enough to store the requested index
     * and to hold the 15 initial seed values. */
    if (cache_len < n) {
        /* The cache must hold indices 0..max(n,15). */
        size_t need = ((size_t)n > 15 ? (size_t)n : 15) + 1;
        if (cache_cap < need) {
            size_t newcap = cache_cap ? cache_cap : 16;
            while (newcap < need) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) {
                /* If reallocation failed, we cannot continue reliably. */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = newcap;
        }

        /* Zero the newly allocated part (unnecessary for arbitrary‑precision
         * types, but harmless). */
        if (cache_len < 15) {
            for (idx_t i = cache_len; i <= 15; ++i) {
                cache[i] = init[i];
            }
            if (cache_len < 15) cache_len = 15;
        }

        /* Coefficients of the recurrence. */
        static const int coeff[14] = {
            5,   3,   9,  -10,  -7,  -166,  -3, -122,
           -71,  165,  43,  -43,  -7,   3
        };

        /* Compute any missing terms up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (int k = 0; k < 14; ++k) {
                idx_t j = i - (idx_t)(k + 1);          /* a(i-(k+1))   */
                seq_t prod = cache[j] * (seq_t)(coeff[k] >= 0 ? coeff[k] : -coeff[k]);
                if (coeff[k] >= 0)
                    sum += prod;
                else
                    sum -= prod;                      /* subtraction works for unsigned/bigint types */
            }
            cache[i] = sum;
        }
        cache_len = n;
    }

    return cache[n];
}
