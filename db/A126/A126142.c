/*
 * A126142  Primes p such that the sum of the decimal digits of p^2 is also a
 *          prime.
 * https://oeis.org/A126142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126142: primes p such that the sum of the decimal digits of p^2 is also prime */
/* This implementation searches for primes incrementally, computing the digit sum of the
 * square of each prime and checking its primality. The sequence is memoised so that
 * any call to a(n) requires no more work than the previous largest n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small prime table for digit sums up to 135. */
static const uint16_t _small_prime_table[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
    101,103,107,109,113,127,131
};
static const size_t _small_prime_count = sizeof(_small_prime_table) / sizeof(_small_prime_table[0]);

/* Test if an integer <= 135 is prime. */
static inline bool is_small_prime(uint16_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < _small_prime_count; ++i) {
        uint16_t p = _small_prime_table[i];
        if (p > x) break;
        if (x == p) return true;
        if (x % p == 0) return false;
    }
    return true;
}

/* Integer square root using 128-bit intermediate to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high; /* floor(sqrt(n)) */
}

/* Static storage for generated primes (used for primality testing). */
static idx_t *gen_primes = NULL;
static size_t   gen_len   = 0;
static size_t   gen_cap   = 0;
static idx_t    next_candidate = 2;    /* next number to test for primality */

/* Static storage for qualifying primes (the sequence itself). */
static seq_t *qual = NULL;
static size_t qlen = 0;
static size_t qcap = 0;

/* Ensure that the array `qual` contains at least `n` terms. */
static void ensure_cache(idx_t n)
{
    if (qlen >= (size_t)n) return;

    /* Grow the cache of qualifying primes until we have enough. */
    while (qlen < (size_t)n) {
        /* Find the next prime. */
        idx_t p = 0;
        for (;;) {
            if (next_candidate < 2) next_candidate = 2;

            /* Primality test using the list of known primes. */
            bool is_prime = true;
            idx_t limit = floor_sqrt_u64(next_candidate);
            for (size_t i = 0; i < gen_len; ++i) {
                idx_t prime = gen_primes[i];
                if (prime > limit) break;
                if (next_candidate % prime == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                p = next_candidate;
                /* Record this prime for future tests. */
                if (gen_len == gen_cap) {
                    size_t new_cap = gen_cap ? gen_cap * 2 : 16;
                    idx_t *tmp = (idx_t *)realloc(gen_primes, new_cap * sizeof(idx_t));
                    if (!tmp) exit(1);
                    gen_primes = tmp;
                    gen_cap = new_cap;
                }
                gen_primes[gen_len++] = p;
                break;
            }
            ++next_candidate;
        }

        /* Compute digit sum of p^2. */
        seq_t sq = (seq_t)p * (seq_t)p;
        seq_t temp = sq;
        seq_t digit_sum = 0;
        while (temp > 0) {
            seq_t d = temp % 10;
            digit_sum += d;
            temp /= 10;
        }
        uint16_t sum = (uint16_t)digit_sum;

        /* If that sum is a prime, store the original prime. */
        if (is_small_prime(sum)) {
            if (qlen == qcap) {
                size_t new_cap = qcap ? qcap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                qual = tmp;
                qcap = new_cap;
            }
            qual[qlen++] = (seq_t)p;
        }

        /* Advance to next candidate number. */
        ++next_candidate;
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid crash */
    ensure_cache(n);
    return qual[n - 1];
}
