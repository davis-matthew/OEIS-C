/*
 * A126164  Sum of the proper exponential divisors of n.
 * https://oeis.org/A126164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = esigma(n) - n, where esigma(n) = product over prime
 *            powers p^r * of (sum_{d|r} p^d). The e‑divisors of n are those
 *            numbers whose prime * exponents divide the corresponding
 *            exponents of n
 * Range:     u8 n <= 395, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assume the following utilities are available in the build:
 *     void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt);
 *     seq_t seq_pow(seq_t base, idx_t exp);
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for the purpose of this sequence */
        return (seq_t)0;

    /* Factorize n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the exponential sigma: product over primes of sum_{d|r} p^d */
    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t r   = exps[i];
        seq_t   sum  = (seq_t)0;

        for (idx_t d = 1; d <= (idx_t)r; ++d) {
            if (r % (uint64_t)d == 0) {
                /* Add p^d to the sum */
                sum += seq_pow((seq_t)p, d);
            }
        }
        sigma *= sum;
    }

    /* esigma(1) = 1; the loop above leaves sigma = 1 for n = 1, which is correct */
    return sigma - (seq_t)n;
}
