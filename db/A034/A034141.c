/*
 * A034141  Number of partitions of n into distinct parts from [1, 11].
 * https://oeis.org/A034141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,11} whose sum is n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently the coefficient of x^n in (1+x)(1+x^2)…(1+x^11)) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache size = max sum 1+…+11 = 66, index 0..66 */
    static seq_t cache[67] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* enumerate all 2^11 subsets */
        for (int mask = 0; mask < (1 << 11); ++mask) {
            unsigned int sum = 0;
            for (int i = 0; i < 11; ++i)
                if (mask & (1 << i))
                    sum += (unsigned int)(i + 1);
            cache[sum]++;      /* cache holds seq_t counts */
        }
        initialized = 1;
    }

    if (n < 67) {
        return cache[(idx_t)n];
    } else {
        /* beyond the maximum possible sum, no partitions */
        return (seq_t)0;
    }
}
