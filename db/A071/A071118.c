/*
 * A071118  Size of the automorphism group of the group Z X Z_n.
 * https://oeis.org/A071118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n * phi(n)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the size of the automorphism group of Z × Z_n. */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* phi(0) is undefined; for safety we return 0 for n == 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t phi_n = phi(n);           /* Euler's totient of n */
    seq_t result = (seq_t)n * phi_n;/* n * phi(n) */
    result *= 2;                    /* multiply by 2 as required */
    return result;
}
