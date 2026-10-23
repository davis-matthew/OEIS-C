/*
 * A143268  a(n) = phi(n)*T(n), where phi(n) is Euler's totient function
 *          (A000010) and T(n) = n*(n+1)/2 is the n-th triangular number
 *          (A000217).
 * https://oeis.org/A143268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*phi(n)/2
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* Compute the n-th triangular number T(n) = n*(n+1)/2. */
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Compute Euler's totient phi(n). */
    seq_t phi_n = phi(n);
    /* Return a(n) = phi(n) * T(n). */
    return phi_n * triangular;
}
