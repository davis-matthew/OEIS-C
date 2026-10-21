/*
 * A062380  a(n) = Sum_{i|n,j|n} phi(i)*phi(j)/phi(gcd(i,j)), where phi is
 *          Euler totient function.
 * https://oeis.org/A062380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062380:
   a(n) = Sum_{d|n} phi(d) * tau(d^2)
   where phi is Euler's totient function and tau(m) is the divisor function.
   This implementation factors n once, enumerates all its divisors,
   and accumulates the required sum.                                      */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper that enumerates all divisors of n.
   `idx`      : current prime index being processed
   `cur_phi`  : phi of the partially built divisor
   `cur_tau`  : tau(d^2) of the partially built divisor
   `pr`       : array of prime factors
   `exp`      : array of exponents for the same primes
   `num`      : number of distinct prime factors
   `sum`      : accumulator for the final result
*/
static void
recurse(size_t idx, seq_t cur_phi, seq_t cur_tau,
        const uint64_t *pr, const uint64_t *exp, size_t num,
        seq_t *sum)
{
    if (idx == num) {
        *sum += cur_phi * cur_tau;
        return;
    }

    const uint64_t p   = pr[idx];
    seq_t          p_seq = (seq_t)p;
    const int     emax = (int)exp[idx];
    seq_t          p_pow = 1;          // p^e for the current e

    for (int e = 0; e <= emax; ++e) {
        seq_t phi_factor;
        if (e == 0) {
            phi_factor = 1;
        } else {
            /* phi(p^e) = p^(e-1)*(p-1)
               p_pow at this point is p^e */
            phi_factor = (p_pow / p_seq) * (p_seq - 1);
        }

        seq_t new_phi = cur_phi * phi_factor;
        seq_t new_tau = cur_tau * (2 * e + 1);

        recurse(idx + 1, new_phi, new_tau, pr, exp, num, sum);

        p_pow *= p_seq;   // advance to p^(e+1) for next iteration
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           // not used by OEIS, keeps function safe

    /* Factorize n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   num = 0;
    factor_u64((uint64_t)n, primes, exps, &num);

    seq_t sum = 0;
    recurse(0, 1, 1, primes, exps, num, &sum);
    return sum;
}
