/*
 * A097555  Number of positive words of length n in the monoid Br_8 of
 *          positive braids on 9 strands.
 * https://oeis.org/A097555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Sequence A097555:
   Number of positive words of length n in the monoid Br_8 of positive braids on 9 strands.
   The sequence satisfies the linear recurrence deduced from the generating function

      G(x) = (1 + x²)⁶ / (1 - 8x + 25x² - 45x³ + 59x⁴ - 57x⁵ + 41x⁶ - 21x⁷ + 7x⁸ - x⁹).

   For n ≥ 9 :

        a(n) = 8 a(n-1) - 25 a(n-2) + 45 a(n-3) - 59 a(n-4)
               + 57 a(n-5) - 41 a(n-6) + 21 a(n-7) - 7 a(n-8) + a(n-9)
               + c(n),

   where c(n) = 0 for odd n and for even n ≤ 12 :

        c(10) = 6   (k = 5 in (1+x²)⁶)
        c(12) = 1   (k = 6 in (1+x²)⁶)

   The first 9 values are given explicitly:
        a(0)=1, a(1)=8, a(2)=45, a(3)=205, a(4)=831, a(5)=3133,
        a(6)=11294, a(7)=39585, a(8)=136302.
*/

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached sequence values */
static idx_t cache_len = 0;         /* index of first unused cache element   */
static idx_t cache_cap = 0;         /* total capacity of the cache           */

static const seq_t init[9] = {
    (seq_t)1, (seq_t)8, (seq_t)45, (seq_t)205,
    (seq_t)831, (seq_t)3133, (seq_t)11294, (seq_t)39585,
    (seq_t)136302
};

/* binomial coefficients C(6,k) for k = 0 .. 6      */
static const unsigned binom6[7] = {
    1, 6, 15, 20, 15, 6, 1
};

seq_t a(idx_t n)
{
    /* ------------------------------------------------------ */
    /*   Ensure the cache has space for indices up to n      */
    /* ------------------------------------------------------ */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache   = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Return a cached value if already available */
    if (n < cache_len) return cache[n];

    /* ------------------------------------------------------ */
    /*            Compute values from cache_len to n           */
    /* ------------------------------------------------------ */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 9) {                     /* initial values */
            val = init[i];
        } else {
            seq_t pos = 8 * cache[i-1];
            pos += 45 * cache[i-3];
            pos += 57 * cache[i-5];
            pos += 21 * cache[i-7];
            pos += cache[i-9];

            seq_t neg = 25 * cache[i-2];
            neg += 59 * cache[i-4];
            neg += 41 * cache[i-6];
            neg += 7 * cache[i-8];

            val = pos - neg;

            /* Add the numerator contribution for  n ≤ 12  */
            if (i <= 12 && (i & 1) == 0) {           /* even n */
                /* i = 2k  →  k = i/2  (0 ≤ k ≤ 6)   */
                val = val + (seq_t)binom6[i >> 1];
            }
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
