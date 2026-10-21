/*
 * A077773  Number of integers between n^2 and (n+1)^2 that are the sum of two
 *          squares; multiple representations are counted once.
 * https://oeis.org/A077773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k with n^2 < k < (n+1)^2 that can be
 *            expressed as a sum of two squares (each k counted only once).
 *            Implementation uses the classical prime–mod‑4 theorem: a number
 *            is representable iff every prime ≡ 3 (mod 4) occurs with an even
 *            exponent in its factorisation
 * Range:     u8 n <= 586, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Minimal prime sieve; keeps a list of primes up to the largest limit seen. */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static uint64_t prime_upto = 0;

static void ensure_primes(uint64_t limit)
{
    if (prime_upto >= limit) return;

    /* Allocate a Boolean sieve up to 'limit'. */
    bool *sieve = (bool*)calloc(limit + 1, sizeof(bool));
    if (!sieve) return;          /* allocation failure – unlikely for n ≤ 10000 */

    /* Eratosthenes sieve. */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!sieve[p]) {
            for (uint64_t q = p * p; q <= limit; q += p)
                sieve[q] = true;
        }
    }

    /* Count primes and rebuild the prime list. */
    size_t new_cnt = 0;
    for (uint64_t p = 2; p <= limit; ++p)
        if (!sieve[p]) ++new_cnt;

    uint64_t *new_primes = (uint64_t*)malloc(new_cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!sieve[p]) new_primes[idx++] = p;
    }

    free(primes);
    free(sieve);

    primes = new_primes;
    prime_cnt = new_cnt;
    prime_upto = limit;
}

/* Test whether a positive integer x is a sum of two squares via the
   classical theorem on primes ≡ 3 (mod 4). */
static bool is_sum_two_squares(uint64_t x)
{
    if (x <= 1) return true;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            size_t exp = 0;
            do {
                x /= p;
                ++exp;
            } while (x % p == 0);
            if ((p & 3ULL) == 3ULL && (exp & 1ULL))
                return false;
        }
    }

    /* If a prime factor > sqrt remains, it must be checked. */
    if (x > 1 && (x & 3ULL) == 3ULL)
        return false;

    return true;
}

/* ==> OEIS sequence A077773 : a(n) = number of k in (n², (n+1)²) that are sums of two squares. */
seq_t a(idx_t n)
{
    /* We need primes up to at least n+1. */
    if (prime_upto < (n + 1))
        ensure_primes((n + 1) * 2);   /* a small buffer to avoid repeated growth */

    seq_t count = 0;

    /* Compute bounds, staying within 64‑bit integer overflow safety. */
    idx_t low  = (idx_t)((seq_t)n * (seq_t)n + 1);
    idx_t high = (idx_t)((seq_t)(n + 1) * (seq_t)(n + 1) - 1);

    for (idx_t k = low; k <= high; ++k) {
        if (is_sum_two_squares((uint64_t)k))
            ++count;
    }

    return count;
}
