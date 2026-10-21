/*
 * A068494  a(n) = n mod phi(n).
 * https://oeis.org/A068494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n mod phi(n) (phi is Euler's totient function)
 * Range:     u8 n <= 529, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* Compute phi(n) */
    seq_t phi_n = phi(n);

    /* Avoid division by zero (phi(0) is undefined, but n>=1) */
    if (phi_n == 0)
        return 0;

    /* Return n mod phi(n) */
    return n_seq % phi_n;
}
