/*
 * A010554  a(n) = phi(phi(n)), where phi is the Euler totient function.
 * https://oeis.org/A010554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = phi(phi(n)) – Euler’s totient applied twice
 * Range:     u8 n <= 555, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes the helper function `phi` is defined in ../utils/phi.h */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* First totient - input is an idx_t, result is a seq_t */
    seq_t first = phi(n);

    /* Second totient - cast back to idx_t since phi(n) <= n */
    seq_t second = phi((idx_t)first);

    return second;
}
