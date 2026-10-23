/*
 * A176095  a(n) = n - phi(2*n), where phi() is the Euler totient A000010().
 * https://oeis.org/A176095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - phi(2*n)
 * Range:     u8 n <= 523, u16 n <= 8926, u32 n <= 8926, u64 n <= 8926,
 *            u128 n <= 8926, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    idx_t two_n = n * 2;
    seq_t phi_val = phi(two_n);
    return ((seq_t)n) - phi_val;
}
