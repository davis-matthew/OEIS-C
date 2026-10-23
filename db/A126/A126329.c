/*
 * A126329  Primes of the form 6p+5 where p is a prime.
 * https://oeis.org/A126329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form 6p+5 where p itself is a prime
 * Range:     u8 n <= 10, u16 n <= 479, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses an incremental search, caching results between calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Primality test for arbitrary‑size seq_t values (provided by the build environment). */
#include "../utils/is_prime_big.h"

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Next candidate for p to test. */
static idx_t next_p = 2;

/* Grow the cache allocation when necessary. */
static void grow_cache_if_needed(void)
{
    if (cache_size >= cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Lightweight deterministic Miller–Rabin for 64‑bit integers, used when seq_t is 64‑bit. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1;
        __uint128_t pow = a;
        uint64_t exp = d;
        while (exp)
        {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool good = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = (x * x) % n;
            if (x == n - 1) { good = true; break; }
        }
        if (!good) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1; return 0 for safety */

    /* Generate terms until we have at least n elements. */
    while (cache_size < n)
    {
        /* Find the next prime p. */
        while (true)
        {
            bool prime;
#ifdef __SIZEOF_INT128__
            if (sizeof(seq_t) <= sizeof(uint64_t))
                prime = is_prime_uint64((uint64_t)next_p);
            else
#endif
                prime = is_prime_big((seq_t)next_p);

            if (prime) break;
            ++next_p;
        }

        /* Compute q = 6p + 5 and test its primality. */
        seq_t q = ((seq_t)6 * (seq_t)next_p) + 5;
        bool qprime;
#ifdef __SIZEOF_INT128__
        if (sizeof(seq_t) <= sizeof(uint64_t))
            qprime = is_prime_uint64((uint64_t)q);
        else
#endif
            qprime = is_prime_big(q);

        /* Cache the term if it is prime. */
        if (qprime)
        {
            grow_cache_if_needed();
            cache[cache_size++] = q;
        }

        /* Move on to the next candidate for p. */
        ++next_p;
    }

    return cache[n - 1];
}
