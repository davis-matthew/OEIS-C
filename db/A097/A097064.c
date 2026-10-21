/*
 * A097064  Expansion of (1 - 4*x + 6*x^2)/(1 - 2*x)^2.
 * https://oeis.org/A097064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)*2^(n-1) for n >= 2 a(0) = 1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It relies on the utility function pow2 from the utils library. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* Compute 2^(n-1) and multiply by (n-1) */
    seq_t two_power = pow2(n - 1);
    return ((seq_t)(n - 1)) * two_power;
}
