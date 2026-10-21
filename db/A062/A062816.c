/*
 * A062816  a(n) = phi(n)*tau(n) - 2n = A000010(n)*A000005(n) - 2*n.
 * https://oeis.org/A062816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n)*tau(n) - 2*n
 * Range:     i8 n <= 71, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* compute φ(n) */
    seq_t phi_n = phi(n);
    /* compute τ(n) */
    seq_t tau_n = (seq_t)divisor_count_u64((uint64_t)n);
    /* compute 2*n in seq_t */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    /* return φ(n)*τ(n) - 2*n */
    return phi_n * tau_n - two_n;
}
