/*
 * A157793  Convolution of numbers of binary zeros and ones.
 * https://oeis.org/A157793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} A023416(i)*A000120(n-i) where A023416(i) is
 *            the number of zeros in the binary expansion of i, and A000120(j)
 *            is the number of ones in the binary expansion of j
 * Range:     u8 n <= 46, u16 n <= 2430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* number of ones in n-i (O(n-i)) */
        seq_t ones = (seq_t)__builtin_popcountll(n - i);

        /* number of zeros in i (Z(i)) */
        seq_t zeros;
        if (i == 0) {
            zeros = 1;               /* binary "0" has one zero */
        } else {
            zeros = (seq_t)(64 - __builtin_clzll(i))
                    - (seq_t)__builtin_popcountll(i);
        }

        sum += zeros * ones;
    }
    return sum;
}
