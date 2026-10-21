/*
 * A071010  Sigma(k)/4 when k is not a sum of 2 squares.
 * https://oeis.org/A071010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(A022544(n))/4 * A022544(n) is the n-th positive
 *            integer that cannot be expressed as a sum of two squares. * The
 *            function returns the sum of divisors of that number divided by 4
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define PRIME_LIMIT 100000
#define MAX_PRIMES 20000

/* Prime sieve – static tables so we compute once. */
static unsigned char is_prime[PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; (size_t)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = 0;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT && primes_cnt < MAX_PRIMES; ++i) {
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    primes_inited = true;
}

/* Test whether n has any prime factor ≡ 3 (mod 4) with odd exponent. */
static bool is_not_sum_of_two_squares(idx_t n)
{
    if (n <= 1) return false;
    idx_t tmp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if ((p & 3ULL) == 3ULL && (exp & 1ULL) == 1ULL) {
            return true;
        }
    }
    if (tmp > 1) {
        if ((tmp & 3ULL) == 3ULL) return true;
    }
    return false;
}

/* Compute sigma(n) = sum of all positive divisors of n. */
static seq_t sigma_of_number(idx_t n)
{
    seq_t sigma = 1;
    idx_t tmp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            seq_t p_pow = 1;          /* p^0 */
            seq_t term_sum = 1;       /* sum of p^0 .. p^e */

            while (tmp % p == 0) {
                tmp /= p;
                p_pow = p_pow * (seq_t)p;     /* p^k */
                term_sum += p_pow;           /* add p^k to sum */
            }
            sigma *= term_sum;
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1) {
        sigma *= ((seq_t)1 + (seq_t)tmp);
    }

    return sigma;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety for off‑by‑one calls */

    init_primes();

    idx_t count = 0;
    idx_t candidate = 0;

    while (count < n) {
        ++candidate;
        if (is_not_sum_of_two_squares(candidate)) {
            ++count;
        }
    }

    seq_t val = sigma_of_number(candidate);
    return val / (seq_t)4;
}
