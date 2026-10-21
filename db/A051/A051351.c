/*
 * A051351  a(n) = a(n-1) + sum of digits of n-th prime.
 * https://oeis.org/A051351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + sumdigits(pₙ),   a(0) = 0
   where pₙ is the n‑th prime (p₁ = 2).  The function caches all
   computed terms and primes so that repeated calls with arbitrary
   indices are fast. */

/* Required standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute sum of decimal digits of a value.  The parameter is an
   unsigned 64‑bit integer (compatible with idx_t). */
static uint64_t digit_sum_uint64(idx_t x)
{
    uint64_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* ---------- Prime generation ---------- */
/* primes[0] is a dummy to make primes[i] the i‑th prime. */
static idx_t *primes = NULL;          /* dynamic array of primes */
static size_t primes_len = 1;          /* current number of elements (including dummy) */
static size_t primes_cap = 16;         /* current capacity */

/* Ensure the prime array contains at least (n+1) elements,
   i.e. primes[1..n] exist. */
static void ensure_primes(size_t n)
{
    if (primes == NULL) {
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        if (!primes)
            return; /* out of memory – the caller will fail gracefully */
        primes[0] = 0;           /* dummy placeholder */
    }

    /* Add primes until we have at least primes_len > n. */
    while (primes_len <= n) {
        idx_t candidate = (primes_len == 1) ? 2 : primes[primes_len - 1] + 1;

        /* Find next prime candidate starting from 'candidate'. */
        while (true) {
            bool is_prime = true;
            for (size_t i = 1; i < primes_len; ++i) {
                idx_t p = primes[i];
                /* Avoid overflow: stop if p > candidate / p. */
                if (p > candidate / p)
                    break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime)
                break; /* candidate is prime */
            ++candidate;
        }

        /* Append the new prime. */
        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap * 2;
            idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
            if (!tmp) /* allocation failed; stop at current size */
                return;
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_len] = candidate;
        ++primes_len;
    }
}

/* ---------- Sequence cache ---------- */
static seq_t *cache = NULL;      /* dynamic array of a(n) values */
static size_t cache_len = 0;     /* number of cached terms (a[0] .. a[cache_len-1]) */

/* Compute the n-th term of A051351.  The function is memoised so
   repeated calls with arbitrary n are fast. */
seq_t a(idx_t n)
{
    /* Initialise cache for a(0). */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0; /* unlikely for small n */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Return a(0) directly. */
    if (n == 0)
        return cache[0];

    /* Extend cache if needed. */
    if ((size_t)n >= cache_len) {
        /* Ensure we have all necessary primes. */
        ensure_primes((size_t)n);

        /* Resize cache to hold up to a(n). */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;

        /* Compute missing terms. */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t sum = (seq_t)digit_sum_uint64(primes[i]); /* primes[i] is the i‑th prime */
            cache[i] = cache[i - 1] + sum;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
