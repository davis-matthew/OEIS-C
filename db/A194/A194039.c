/*
 * A194039  Sum of nonprime divisors of n^2 + 1.
 * https://oeis.org/A194039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of nonprime divisors of n^2 + 1 a(n) = A023890(n^2 +
 *            1) = σ(n^2+1) – sum of distinct prime divisors of n^2+1
 * Range:     u8 n <= 12, u16 n <= 186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Work with a 64‑bit integer for the intermediate value n²+1 */
    uint64_t nn  = (uint64_t)n;
    uint64_t m   = nn * nn + 1ULL;

    /* temp holds the remaining factor to be processed */
    uint64_t temp   = m;

    /* σ(m) – product over prime powers (p^(e+1)-1)/(p-1)  */
    seq_t sigma   = (seq_t)1;

    /* Sum of the distinct prime divisors of m */
    seq_t prime_sum = (seq_t)0;

    /* Factor 2 separately */
    if ((temp & 1ULL) == 0) {
        unsigned exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++exp;
        }
        prime_sum += (seq_t)2;

        seq_t term = (seq_t)1;
        seq_t pow  = (seq_t)1;
        for (unsigned k = 1; k <= exp; ++k) {
            pow  *= (seq_t)2;
            term += pow;
        }
        sigma *= term;
    }

    /* Factor odd primes: 3,5,7,…   */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            prime_sum += (seq_t)p;

            seq_t term = (seq_t)1;
            seq_t pow  = (seq_t)1;
            for (unsigned k = 1; k <= exp; ++k) {
                pow  *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
    }

    /* If a prime factor > sqrt remains, it occurs to the first power */
    if (temp > 1) {
        prime_sum += (seq_t)temp;
        sigma *= ((seq_t)1 + (seq_t)temp);
    }

    /* Non‑prime divisors = σ – sum of distinct primes */
    return sigma - prime_sum;
}
