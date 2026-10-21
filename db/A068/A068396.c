/*
 * A068396  n-th prime minus its reversal.
 * https://oeis.org/A068396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, nice, look
 * Formula:   a(n) = prime(n) – reverse(prime(n)) (OEIS A068396)
 * Range:     i8 n <= 26, i16 n <= 1229, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Reverse the decimal digits of an unsigned 64‑bit integer. */
static seq_t rev_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return (seq_t)rev;
}

/* Static cache of primes generated on demand.  */
static seq_t *prime_cache   = NULL; /* in order, 1st element = 2 */
static size_t prime_count   = 0;     /* number of primes in the cache */
static size_t prime_cap     = 0;     /* allocated capacity of the cache */

/* Ensure that the cache holds at least `n` primes (1‑based index). */
static void ensure_primes(idx_t n)
{
    if ((idx_t)prime_count >= n) return;

    /* Grow the cache if necessary. */
    if (prime_cap == 0) {
        prime_cap   = 16;
        prime_cache = (seq_t *)malloc(prime_cap * sizeof(seq_t));
    }
    if (prime_count >= prime_cap) {
        prime_cap *= 2;
        prime_cache = (seq_t *)realloc(prime_cache, prime_cap * sizeof(seq_t));
    }

    /* Start generating from just after the last cached prime. */
    uint64_t candidate = (prime_count == 0) ? 2
                                      : (uint64_t)prime_cache[prime_count - 1] + 1;

    /* Continue until we have `n` primes. */
    while (prime_count < (size_t)n) {
        bool is_prime = true;
        uint64_t pc     = candidate;

        /* Trial division by all cached primes <= sqrt(candidate). */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = (uint64_t)prime_cache[i];
            if (p * p > pc) break;
            if (pc % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count >= prime_cap) {
                prime_cap *= 2;
                prime_cache = (seq_t *)realloc(prime_cache, prime_cap * sizeof(seq_t));
            }
            prime_cache[prime_count++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* --- Public function required by the build system --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n=0, safe default */
    ensure_primes(n);
    seq_t prime = prime_cache[n - 1];            /* nth prime (1‑based) */
    seq_t rev   = rev_uint64((uint64_t)prime);    /* decimal reversal */
    return prime - rev;                           /* a(n) */
}
