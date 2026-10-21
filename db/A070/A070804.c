/*
 * A070804  Number of primes not exceeding phi(n).
 * https://oeis.org/A070804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(phi(n)) (number of primes ≤ φ(n))
 * Range:     u8 n <= 494, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a trial‑division φ(n) and a lazy sieve for primes. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of prime sieve flags */
static unsigned char *prime_flag = NULL;   /* 0/1 flags for numbers up to the current limit */
static size_t sieve_limit = 0;

/* build a simple sieve up to 'limit' (inclusive) */
static void build_sieve(size_t limit)
{
    if (prime_flag) free(prime_flag);
    /* allocate flags for numbers 0..limit */
    prime_flag = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    if (!prime_flag) exit(1);   /* out of memory */

    /* mark all ≥2 as potentially prime */
    for (size_t i = 2; i <= limit; ++i) prime_flag[i] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                prime_flag[j] = 0;
            }
        }
    }
    sieve_limit = limit;
}

/* return number of primes ≤ n  (n ≤ sieve_limit) */
static int prime_count_upto(size_t n)
{
    if (n > sieve_limit) build_sieve(n);
    int cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (prime_flag[i]) ++cnt;
    return cnt;
}

/* Euler phi: O(√n) trial division, suitable for n ≤ 10⁴ */
static seq_t phi_of(idx_t n)
{
    if (n < 1) return 0;
    seq_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) /* remaining prime factor */
        result -= result / m;
    return result;
}

/* required function */
seq_t a(idx_t n)
{
    seq_t phi = phi_of(n);
    /* phi fits into idx_t for the tested range (≤10⁴) */
    idx_t phi_int = (idx_t)phi;
    return (seq_t)prime_count_upto(phi_int);
}
