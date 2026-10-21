/*
 * A084301  a(n) = sigma(n) mod 6.
 * https://oeis.org/A084301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 6
 * Range:     u8 n <= 2578, u16 n <= 9635, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of n modulo 6.
   Uses a small prime sieve for factorisation. */

#include <stdbool.h>
#include <stddef.h>

#define PRIME_SIEVE_LIMIT 100000   /* enough for n up to ~10^10 */

static bool primes_ready = false;
static int primes[PRIME_SIEVE_LIMIT];
static int prime_count = 0;

/* Initialise a static sieve of primes up to PRIME_SIEVE_LIMIT */
static void init_primes(void)
{
    if (primes_ready)
        return;

    static bool composite[PRIME_SIEVE_LIMIT + 1];
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_count++] = i;
            if ((long long)i * i <= PRIME_SIEVE_LIMIT) {
                for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* Compute sigma(n) (sum of divisors) for unsigned integer n. */
static seq_t sigma_of(idx_t n)
{
    init_primes();

    seq_t result = 1;
    idx_t temp = n;

    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if ((unsigned long long)p * (unsigned long long)p > temp)
            break;
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* compute 1 + p + p^2 + ... + p^exp */
            seq_t term = 1;
            seq_t p_pow = 1;
            for (idx_t e = 1; e <= exp; ++e) {
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            result *= term;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        result *= (seq_t)(1 + temp);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    return s % 6;
}
