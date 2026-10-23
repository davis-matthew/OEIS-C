/*
 * A014342  Convolution of primes with themselves.
 * https://oeis.org/A014342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} prime(i) * prime(n+1-i)
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 826, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches the first n primes in a static array and
   recomputes a(n) as the convolution of those primes with themselves.
   The cache persists across calls so the cost is amortized. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of the first primes */
static uint32_t *prime_cache = NULL;     /* array of primes */
static idx_t prime_cache_size = 0;       /* number of primes stored */

/* ensure that at least n primes are available in prime_cache */
static void ensure_primes(idx_t n)
{
    if (prime_cache_size >= n)
        return;

    /* grow the array */
    uint32_t *new_cache = (uint32_t *)realloc(prime_cache, n * sizeof(uint32_t));
    if (!new_cache) abort();
    prime_cache = new_cache;

    idx_t count = prime_cache_size;   /* primes already present */

    /* seed with the first prime if necessary */
    if (count == 0) {
        prime_cache[count++] = 2;
    }

    /* next candidate to test */
    uint64_t candidate = prime_cache[count - 1];
    if (candidate < 2) candidate = 2;
    candidate++;

    /* add primes until we have n of them */
    while (count < n) {
        /* skip even numbers greater than 2 */
        if (candidate > 2 && !(candidate & 1ULL))
            ++candidate;

        /* test divisibility by known primes up to sqrt(candidate) */
        bool is_prime = true;
        uint64_t limit = (uint64_t)(candidate * candidate) > candidate ? 0 : candidate; /* dummy */
        /* loop over primes */
        for (idx_t j = 0; j < count; ++j) {
            uint32_t p = prime_cache[j];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            if (p2 > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (candidate > UINT32_MAX) {
                /* cannot store as uint32_t; abort for practical limits */
                abort();
            }
            prime_cache[count++] = (uint32_t)candidate;
        }
        ++candidate;
    }
    prime_cache_size = count;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_primes(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p1 = (seq_t)prime_cache[i - 1];
        seq_t p2 = (seq_t)prime_cache[n - i];
        sum += p1 * p2;
    }
    return sum;
}
