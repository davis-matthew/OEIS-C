/*
 * A129224  Residues of the Lucas - Lehmer primality test for M(23) = 8388607.
 * https://oeis.org/A129224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(0) = 4, a(n) = (a(n-1)^2 - 2) mod (2^23 - 1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t M = (seq_t)8388607u;     /* 2^23 - 1 */
    const seq_t M_minus_2 = M - 2;

    if (n == 0)
        return (seq_t)4u;

    seq_t val = (seq_t)4u;
    for (idx_t i = 1; i <= n; ++i) {
        val = ((val * val) % M + M_minus_2) % M;
    }
    return val;
}
