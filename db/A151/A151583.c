/*
 * A151583  Number of permutations of 2 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 adjacent element pairs in
 *          decreasing order.
 * https://oeis.org/A151583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^n - 4*n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t ns = (seq_t)n;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= 3;
    seq_t four_n = (seq_t)4 * ns;
    seq_t diff = pow3 - four_n;
    return ns * diff;
}
