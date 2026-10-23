/*
 * A158302  "1" followed by repeats of 2^k deleting all 4^k, k > 0.
 * https://oeis.org/A158302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n) if n is odd; a(n) = 2^(n-1) if n is even and n > 0;
 *            a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is 1, 2, 2, 8, 8, 32, 32, … – powers of 2 repeated twice, skipping powers of 4. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* For n > 0, exponent depends on parity of n */
    idx_t exp = (n & 1) ? n : (n - 1);
    return pow2(exp);
}
