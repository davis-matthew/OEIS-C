/*
 * A109217  a(n) is the binary string of length n+1 that has 0's at indices
 *          that are squares and 1's elsewhere, where the most significant
 *          digit has index 0.
 * https://oeis.org/A109217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = decimal number whose decimal digits form the binary string
   (with 0 at perfect square indices) of length n‑1 after discarding
   the leading two zeros (indices 0 and 1).  Computed by iterating over
   indices 2..n and building the number digit by digit. */
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    seq_t val = 0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t s = int_sqrt(i);
        seq_t bit = (s * s == i) ? 0 : 1;
        val = val * 10 + bit;
    }
    return val;
}
