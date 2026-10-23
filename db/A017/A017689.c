/*
 * A017689  Numerator of sum of -13th powers of divisors of n.
 * https://oeis.org/A017689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma_13(n) / gcd(sigma_13(n), n^13)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 920,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not used, but guard against zero */
        return (seq_t)0;

    /* factorize n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma = (seq_t)1;          /* sigma_13(n) */
    const uint64_t K = 13ULL;        /* exponent used in the sequence */

    for (size_t i = 0; i < cnt; ++i) {
        /* term contribution: 1 + p^13 + p^26 + ... + p^(13*e) */
        seq_t term = (seq_t)1;
        seq_t base_pow = big_pow((seq_t)primes[i], (seq_t)K);  /* p^13 */
        seq_t p_pow = (seq_t)1;
        for (uint64_t j = 1; j <= exps[i]; ++j) {
            p_pow *= base_pow;          /* p^(13*j) */
            term += p_pow;
        }
        sigma *= term;
    }

    /* n^13 */
    seq_t n_pow_13 = big_pow((seq_t)n, (seq_t)13ULL);

    /* reduction: a(n) = sigma / gcd(sigma, n^13) */
    seq_t g = _gcd_seq_t(sigma, n_pow_13);
    return sigma / g;
}
