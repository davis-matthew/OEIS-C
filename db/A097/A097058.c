/*
 * A097058  Numbers of the form p^2 + 2^p for p prime.
 * https://oeis.org/A097058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n^2 + 2^p_n, where p_n is the n-th prime number
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple primality test (sufficient for n up to about 1e6). */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Grow a dynamically allocated array. */
static void ensure_size(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Power of 2: 2^e computed by repeated multiplication. */
static seq_t pow2(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 2;
    return r;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* Local static state: cached primes and sequence terms. */
    static uint64_t *primes = NULL;      /* primes[0] == 2, primes[1] == 3, ...     */
    static size_t primes_sz = 0, primes_cap = 0;
    static seq_t  *cache  = NULL;       /* cached terms a(1), a(2), ...           */
    static size_t cache_sz = 0, cache_cap = 0;
    static uint64_t candidate = 2;      /* next number to test for primality      */

    /* Generate primes until we have at least n primes. */
    while (primes_sz < n) {
        /* Find next prime. */
        while (!is_prime(candidate))
            ++candidate;
        /* Store it. */
        ensure_size((void **)&primes, &primes_cap, primes_sz + 1, sizeof(uint64_t));
        primes[primes_sz++] = candidate;
        ++candidate;
    }

    /* Compute sequence terms until we have a(n). */
    while (cache_sz < n) {
        uint64_t p = primes[cache_sz];          /* p_n for the term being computed */
        seq_t term = (seq_t)p * (seq_t)p;       /* p^2   */
        term += pow2(p);                       /* + 2^p */
        ensure_size((void **)&cache, &cache_cap, cache_sz + 1, sizeof(seq_t));
        cache[cache_sz++] = term;
    }

    /* Return the requested term (sequence is 1‑based). */
    return cache[n - 1];
}
