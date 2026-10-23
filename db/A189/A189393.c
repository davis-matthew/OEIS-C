/*
 * A189393  a(n) = phi(n^4).
 * https://oeis.org/A189393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3*phi(n)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* cast to seq_t to avoid overflow in intermediate types */
    seq_t n_seq = (seq_t)n;
    seq_t n3 = n_seq * n_seq * n_seq;   /* n^3 in seq_t */
    seq_t phi_n = phi(n);                /* φ(n) */
    return n3 * phi_n;                   /* n^3 * φ(n) = φ(n^4) */
}
