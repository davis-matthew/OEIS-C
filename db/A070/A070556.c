/*
 * A070556  a(n) = cototient(totient(n)).
 * https://oeis.org/A070556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = φ(n) – φ(φ(n)) (from A070556)
 * Range:     u8 n <= 377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the public φ implementation from "../utils/phi.h" */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* φ(n) */
    seq_t phi_n = phi(n);
    /* φ(φ(n)).  phi() takes an idx_t argument, so cast safely. */
    seq_t phi_phi = phi((idx_t)phi_n);
    /* a(n) = φ(n) – φ(φ(n)) */
    return phi_n - phi_phi;
}
