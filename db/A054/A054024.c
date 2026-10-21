/*
 * A054024  Sum of the divisors of n reduced modulo n.
 * https://oeis.org/A054024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) mod n (sum of divisors of n modulo n)
 * Range:     u8 n <= 294, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1, but guard against 0 */
    if (n == 0) return 0;
    const seq_t mod = (seq_t)n;
    seq_t sigma = 1 % mod;          /* product of divisor-sum factors mod n */
    uint64_t temp = n;
    uint64_t p = 2;

    /* trial division */
    while ((unsigned __int128)p * p <= temp) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* compute 1 + p + p^2 + ... + p^e  (mod n) */
            seq_t term_sum = 1 % mod;
            seq_t p_power = 1 % mod;
            for (uint64_t i = 0; i < e; ++i) {
                p_power = (seq_t)((unsigned __int128)p_power * p % mod);
                term_sum = (seq_t)((unsigned __int128)term_sum + p_power % mod);
                term_sum %= mod;
            }

            /* multiply into overall sigma  (mod n) */
            sigma = (seq_t)((unsigned __int128)sigma * term_sum % mod);
        }
        ++p;
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t term_sum = ((seq_t)1 + (seq_t)temp) % mod; /* 1 + p */
        sigma = (seq_t)((unsigned __int128)sigma * term_sum % mod);
    }

    return sigma;
}
