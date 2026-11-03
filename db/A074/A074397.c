/*
 * A074397  Number of primes between sigma(n) and n exclusive.
 * https://oeis.org/A074397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes: a(n) = number of primes p with n < p < sigma(n)
 *  (sigma(n) is the sum of all divisors of n)
 * The implementation uses simple trial division for factorisation and
 * a basic primality test, which is fast enough for n up to about 10000.
 */
#include <stdint.h>
#include <stdbool.h>

/* compute sigma(n) by trial division */
static seq_t sigma_of_uint(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n;
    seq_t result = 1;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            /* term = (p^(e+1)-1)/(p-1) */
            seq_t p_pow = 1;
            for (int i = 0; i < e + 1; ++i) p_pow *= (seq_t)p;
            seq_t term = (p_pow - 1) / ((seq_t)p - 1);
            result *= term;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        result *= (seq_t)m + 1;  /* (p^2 - 1)/(p - 1) = p + 1 when e = 1 */
    }
    return result;
}

/* Simple primality test, works for idx_t values up to 2^31  */
static bool is_prime_simple(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (idx_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

/* count primes between n and sigma(n) exclusive */
static seq_t primes_between(idx_t n, seq_t sigma)
{
    seq_t count = 0;
    for (idx_t k = n + 1; (seq_t)k < sigma; ++k) {
        if (is_prime_simple(k)) ++count;
    }
    return count;
}

/* required external function */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_of_uint(n);
    return primes_between(n, sigma);
}
