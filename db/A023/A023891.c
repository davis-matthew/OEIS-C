/*
 * A023891  Sum of composite divisors of n.
 * https://oeis.org/A023891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of composite divisors of n
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) can also be calculated as A023890(n) - 1, where A023890(n) is the sum
   of all positive divisors of n. This implementation uses prime factorisation
   to compute σ(n) (sum of all divisors) and subtracts 1 and the sum of the
   distinct prime divisors. */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)                 /* a(1) = 0 */
        return 0;

    idx_t m = n;
    seq_t sigma = 1;           /* product of divisor sums for each prime factor */
    seq_t sum_prime = 0;       /* sum of distinct prime divisors */

    /* factor 2 separately (most common small prime) */
    if (m % 2 == 0) {
        idx_t k = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++k;
        }
        seq_t term = pow_seq((seq_t)2, (idx_t)k + 1) - 1;   /* (2^(k+1) - 1) */
        sigma *= term;
        sum_prime += (seq_t)2;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            seq_t pow_val = pow_seq((seq_t)p, (idx_t)k + 1);           /* p^(k+1) */
            seq_t term = (pow_val - 1) / (((seq_t)p) - 1);           /* σ_p^k */
            sigma *= term;
            sum_prime += (seq_t)p;
        }
    }

    /* any remaining prime factor > sqrt(n) */
    if (m > 1) {
        seq_t term = (seq_t)m + 1;          /* σ_p^1 = 1 + p */
        sigma *= term;
        sum_prime += (seq_t)m;
    }

    /* composite divisors sum = σ(n) - sum_of_primes - 1 */
    return sigma - sum_prime - 1;
}
