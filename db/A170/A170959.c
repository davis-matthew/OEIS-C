/*
 * A170959  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 6.
 * https://oeis.org/A170959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {3,7,11,15,19,23} whose sum is n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Exponents of x in the product (1 + x^(4*i-1)), i=1..6 */
    static const int exps[6] = {3, 7, 11, 15, 19, 23};

    /* Precomputed subset sums for all 2^6 subsets */
    static seq_t subset_sums[64];
    static bool initialized = false;

    if (!initialized) {
        for (int mask = 0; mask < 64; ++mask) {
            seq_t sum = 0;
            for (int i = 0; i < 6; ++i) {
                if (mask & (1 << i))
                    sum += (seq_t)exps[i];
            }
            subset_sums[mask] = sum;
        }
        initialized = true;
    }

    /* Maximum possible sum is 3+7+11+15+19+23 = 78 */
    if (n > 78)
        return 0;

    seq_t count = 0;
    for (int mask = 0; mask < 64; ++mask) {
        if (subset_sums[mask] == (seq_t)n)
            ++count;
    }
    return count;
}
