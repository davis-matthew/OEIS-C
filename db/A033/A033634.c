/*
 * A033634  OddPowerSigma(n) = sum of odd power divisors of n.
 * https://oeis.org/A033634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime factors p^r of n of * 1 + (p^(s+2) -
 *            p) / (p^2 - 1) , * where s = r if r is odd, and s = r-1 if r is
 *            even. * This equals the sum of divisors of n whose prime
 *            exponents are all odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 6386, u64 n <= 9691,
 *            u128 n <= 9691, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle the first term n==1 separately */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Factor n into primes and exponents. */
    /* We only need a few factors (max about 64 for 64‑bit numbers). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t r   = (seq_t)exps[i];
        seq_t s   = (r & 1) ? r : r - 1;        /* s = r if odd, else r-1 */
        seq_t exp = s + 2;                     /* exponent for p^(s+2) */

        seq_t pow_val = big_pow(p, exp);
        seq_t numerator   = pow_val - p;
        seq_t denominator = p * p - 1;

        seq_t term = (seq_t)1 + numerator / denominator;
        result *= term;
    }

    return result;
}
