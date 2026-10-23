/*
 * A022157  a(n) = n^2 - phi(n)*tau(n)^2.
 * https://oeis.org/A022157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = n^2 - phi(n) * tau(n)^2
 * Range:     i8 n <= 16, i16 n <= 190, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* cast index to seq_t for calculation */
    seq_t ns = (seq_t)n;
    /* n^2 */
    seq_t nsq = ns * ns;
    /* phi(n) */
    seq_t ph = phi(n);
    /* tau(n) = number of divisors of n */
    uint64_t tau_u = divisor_count_u64(n);
    seq_t tau = (seq_t)tau_u;
    /* tau(n)^2 */
    seq_t tau2 = tau * tau;
    /* result */
    seq_t res = nsq - ph * tau2;
    return res;
}
