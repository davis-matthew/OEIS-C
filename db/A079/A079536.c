/*
 * A079536  a(n) = phi(n)*d(n) - sigma(n).
 * https://oeis.org/A079536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n)*d(n) - sigma(n)
 * Range:     i8 n <= 76, i16 n <= 4031, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the supplied helper functions:
 *   - phi(idx_t n)            : Euler's totient function
 *   - sigma_of(idx_t n)       : sum of divisors function
 *   - divisor_count_u64(idx_t n) : number of divisors of n
 *
 * All operations are performed with the seq_t type to avoid overflow.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t phi_n   = phi(n);                /* φ(n) */
    uint64_t d_cnt = divisor_count_u64(n); /* d(n) */
    seq_t sigma_n = sigma_of(n);           /* σ(n) */

    return phi_n * (seq_t)d_cnt - sigma_n;
}
