/*
 * A194187  Difference of n-th prime and (bitwise XOR of n and n-th prime).
 * https://oeis.org/A194187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A000040(n) - (A070883(n) = n XOR A000040(n))
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime using a simple sieve and returns a(n) = p_n - (n XOR p_n) */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static cache for primes */
static seq_t *prime_cache = NULL;      /* array of primes */
static idx_t  prime_count = 0;         /* number of primes stored */
static idx_t  last_limit  = 0;         /* sieve limit that produced the cache */

/* helper: recompute primes up to at least p there are at least n primes */
static void recompute_primes(idx_t n)
{
    /* limit estimate: for n <= 5 use 15, else n*20 comfortably exceeds the nth prime for n <= 10000 */
    idx_t limit = n * 20;
    if (limit < 15) limit = 15;

    /* if already computed up to an equal or larger limit, nothing to do */
    if (limit <= last_limit) return;

    /* sieve of Eratosthenes over indices 0..limit */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) abort();           /* out‑of‑memory */
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p) {
                is_prime[q] = 0;
            }
        }
    }

    /* count primes and allocate array */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++count;
    }

    seq_t *new_cache = (seq_t *)malloc((size_t)count * sizeof(seq_t));
    if (!new_cache) abort();

    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            new_cache[idx++] = (seq_t)i;
        }
    }

    /* replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_count = count;
    last_limit  = limit;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but guard */

    /* ensure we have at least n primes cached */
    if (prime_count < n) recompute_primes(n);

    seq_t p = prime_cache[n - 1];          /* nth prime (p_1 = 2) */
    seq_t x = (seq_t)n;                    /* n as seq_t */
    seq_t xor_val = p ^ x;                 /* n XOR p */
    return p - xor_val;                    /* a(n) */
}
