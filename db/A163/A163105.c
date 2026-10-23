/*
 * A163105  a(n) = tau(sigma(tau(n))), where tau = number of divisors of n
 *          (A000005), and sigma = sum of divisors of n (A000203).
 * https://oeis.org/A163105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000203(A000005(n))) = number of divisors of (sum
 *            of divisors of (number of divisors of n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* no term for n=0, but safe anyway */
    /* tau(n) */
    uint64_t tau1 = divisor_count_u64((uint64_t)n);
    /* sigma(tau(n)) – sigma_of works on idx_t, so cast tau1 */
    seq_t sigma_tau = sigma_of((idx_t)tau1);
    /* tau(sigma(tau(n))) – the result of sigma_tau fits in uint64_t
       for the range of n we are interested in (n up to about 10⁴). */
    uint64_t tau2 = divisor_count_u64((uint64_t)sigma_tau);
    return (seq_t)tau2;
}
