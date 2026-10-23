/*
 * A121048  a(n) = n + phi(n), where phi is the Euler totient function.
 * https://oeis.org/A121048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + phi(n)
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t phi_n  = phi(n);
    return n_seq + phi_n;
}
