/*
 * A170960  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 7.
 * https://oeis.org/A170960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=1..7} (1 + x^(4*i-1))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (number of subsets of {3,7,11,15,19,23,27} with sum n) */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t coeff[106];
    static bool initialized = false;
    if (!initialized) {
        /* Exponent list: 4*i-1 for i=1..7 */
        const idx_t exps[7] = { 3, 7, 11, 15, 19, 23, 27 };
        /* Compute all subset sums */
        for (idx_t mask = 0; mask < (idx_t)1 << 7; ++mask) {
            idx_t sum = 0;
            for (int i = 0; i < 7; ++i) {
                if (mask & ((idx_t)1 << i))
                    sum += exps[i];
            }
            /* sum is in [0,105] */
            coeff[sum] += (seq_t)1;
        }
        initialized = true;
    }
    if (n <= 105)
        return coeff[(size_t)n];
    return (seq_t)0;
}
