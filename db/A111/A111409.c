/*
 * A111409  a(n) = f(f(n+1)) - f(f(n)), where f(0)=0, and for m>0, f(m) =
 *          phi(m) = A000010(m).
 * https://oeis.org/A111409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(phi(n+1)) - phi(phi(n))
 * Range:     i8 n <= 357, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_phi.h"

seq_t a(idx_t n)
{
    /* phi(n) for n>0; phi(0)=0 by definition of f */
    seq_t phi_n = (n == 0) ? 0 : (seq_t)compute_phi((uint64_t)n);
    seq_t phi_n1 = (seq_t)compute_phi((uint64_t)(n + 1));

    /* phi(phi(n)) may be 0 only if phi(n)=0 (i.e., n=0) */
    seq_t phi_phi_n  = (phi_n == 0) ? 0 : (seq_t)compute_phi((uint64_t)phi_n);
    seq_t phi_phi_n1 = (seq_t)compute_phi((uint64_t)phi_n1);

    return phi_phi_n1 - phi_phi_n;
}
