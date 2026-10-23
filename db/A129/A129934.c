/*
 * A129934  Numerators of partial sums of a series for the inverse of the
 *          arithmetic-geometric mean (agM) of sqrt(2)/2 and 1.
 * https://oeis.org/A129934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of ∑_{j=0..n} C(2j,j)^2 / 2^(5j)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;
    static idx_t   last_n = 0;      /* highest index already computed */

    /* state for the incremental evaluation of the series */
    static seq_t bin_val = 1;       /* current central binomial coefficient C(2j,j) */
    static seq_t sum     = 1;       /* numerator of the sum with denominator 2^K */
    static idx_t K        = 0;      /* current exponent of 2 in the denominator */

    /* initialise the cache once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = sum;             /* a(0) = 1 */
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if requested value already computed, return it */
    if (n <= last_n)
        return cache[(size_t)n];

    /* compute successive terms from last_n+1 up to n */
    for (idx_t j = last_n + 1; j <= n; ++j) {
        /* update central binomial coefficient:
           C(2j,j) = C(2(j-1),j-1) * (4j-2) / j   (exact division) */
        seq_t mult = (seq_t)(4 * j - 2);
        seq_t num  = bin_val * mult;
        bin_val    = num / (seq_t)j;

        /* exponent of 2 in bin_val */
        seq_t tmp   = bin_val;
        idx_t exp2  = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp2;
        }

        /* odd part of the binomial coefficient */
        seq_t odd  = bin_val >> exp2;
        seq_t odd2 = odd * odd;

        /* exponent of 2 in the denominator of the current term */
        idx_t e = 5 * j - 2 * exp2;

        /* align the denominator of the accumulated sum */
        if (e > K) {
            sum <<= (e - K);   /* increase denominator */
            K    = e;
        }

        /* add the term */
        sum += odd2 << (K - e);

        /* reduce the fraction by removing common powers of two */
        idx_t tinz = 0;
        while ((sum & 1) == 0) {
            sum >>= 1;
            ++tinz;
        }
        K -= tinz;

        /* store the numerator of the reduced fraction */
        cache[(size_t)j] = sum;
    }

    last_n = n;
    return cache[(size_t)n];
}
