/*
 * A047714  Sums of 3 but no fewer nonzero fourth powers.
 * https://oeis.org/A047714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer that can be expressed as a sum
 *            of three non‑zero fourth powers (i.e. k^4 + l^4 + m^4 with k,l,m
 *            ≥ 1), but cannot be expressed as a sum of one or two such fourth
 *            powers
 * Range:     u8 n <= 10, u16 n <= 570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t fourth_power(idx_t x)
{
    /* use 128‑bit intermediate to avoid overflow for moderately large x */
    return (idx_t)((__int128)x * x * x * x);
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; protect against 0 */
        return (seq_t)0;

    /* Start with a modest limit and double until enough terms are found. */
    idx_t limit = 16;

    while (1) {
        /* Determine the largest a such that a^4 ≤ limit. */
        idx_t max_a = 0;
        for (idx_t a = 1;; ++a) {
            idx_t p = fourth_power(a);
            if (p > limit)
                break;
            max_a = a;
        }
        if (max_a == 0) {
            /* limit too small; increase and retry */
            limit <<= 1;
            continue;
        }

        /* Allocate bit arrays to mark sums.  Use unsigned char for compactness. */
        size_t sz = (size_t)(limit + 1);
        unsigned char *sum12 = (unsigned char*)calloc(sz, 1);   /* sums of 1 or 2 fourth powers */
        unsigned char *sum3  = (unsigned char*)calloc(sz, 1);   /* sums of exactly 3 fourth powers */
        if (!sum12 || !sum3) {
            free(sum12);
            free(sum3);
            return (seq_t)0;   /* allocation failure */
        }

        /* Store fourth powers. */
        idx_t *powers = (idx_t*)malloc((size_t)(max_a + 1) * sizeof(idx_t));
        if (!powers) {
            free(sum12);
            free(sum3);
            return (seq_t)0;
        }
        powers[0] = 0;                    /* unused */
        for (idx_t i = 1; i <= max_a; ++i)
            powers[i] = fourth_power(i);

        /* Mark sums of one or two fourth powers. */
        for (idx_t i = 1; i <= max_a; ++i) {
            idx_t a4 = powers[i];
            sum12[a4] = 1;
            for (idx_t j = 1; j <= max_a; ++j) {
                idx_t sum = a4 + powers[j];
                if (sum > limit)
                    break;
                sum12[sum] = 1;
            }
        }

        /* Mark sums of exactly three fourth powers. */
        for (idx_t i = 1; i <= max_a; ++i) {
            idx_t a4 = powers[i];
            for (idx_t j = 1; j <= max_a; ++j) {
                idx_t sum2 = a4 + powers[j];
                if (sum2 > limit)
                    break;
                for (idx_t k = 1; k <= max_a; ++k) {
                    idx_t sum3_val = sum2 + powers[k];
                    if (sum3_val > limit)
                        break;
                    sum3[sum3_val] = 1;
                }
            }
        }

        /* Scan increasing values and count valid sums. */
        idx_t count = 0;
        for (idx_t val = 1; val <= limit; ++val) {
            if (sum3[val] && !sum12[val]) {
                ++count;
                if (count == n) {
                    seq_t result = (seq_t)val;
                    free(sum12);
                    free(sum3);
                    free(powers);
                    return result;
                }
            }
        }

        /* Not enough terms yet: free arrays, double the limit, and retry. */
        free(sum12);
        free(sum3);
        free(powers);
        limit <<= 1;  /* double the limit */
    }
}
