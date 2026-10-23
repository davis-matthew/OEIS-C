/*
 * A154510  Primes of the form 648*k^2 + 72*k + 1.
 * https://oeis.org/A154510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 513, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of the form 648*k^2 + 72*k + 1
   a(n) = the n‑th prime among the numbers 648*k^2 + 72*k + 1
   (k starts at 0).  a(1) = 10657, a(2) = 16561, etc. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers (John‑Graham–Smail 2004). */
static bool is_prime_u64(uint64_t n) {
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    static const int num_small = sizeof(small_primes)/sizeof(small_primes[0]);

    if (n < 2) return false;
    for (int i = 0; i < num_small; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* Write n-1 as d*2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Test bases sufficient for 64-bit integers. */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    static const int num_bases = sizeof(bases)/sizeof(bases[0]);

    for (int i = 0; i < num_bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;

        __uint128_t x = 1, pow = a;
        uint64_t e = d;
        /* binary exponentiation modulo n */
        while (e) {
            if (e & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool passed = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}

/* Cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static idx_t prime_count = 0;
static idx_t next_k = 0;   /* next k to test */

/* Compute the nth term (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* defensive guard, not used */

    /* Grow the cache until it contains at least n primes. */
    while (prime_count < n) {
        /* Find the next prime of the desired form. */
        while (1) {
            uint64_t k = (uint64_t)next_k;                     /* promote to 64 bit */
            unsigned __int128 val128 =
                (unsigned __int128)648 * k * k
              + (unsigned __int128)72  * k
              + (unsigned __int128)1;
            uint64_t val = (uint64_t)val128;                    /* fits 64‑bit for all needed n */
            ++next_k;                                           /* advance for next iteration */

            if (is_prime_u64(val)) {                           /* prime found */
                /* Resize cache if necessary. */
                if (cache_cap <= prime_count) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();                         /* out of memory */
                    prime_cache = tmp;
                    cache_cap = new_cap;
                }
                prime_cache[prime_count++] = (seq_t)val;
                break;                                          /* exit inner loop */
            }
        }
    }

    return prime_cache[n - 1];   /* zero‑based indexing in cache */
}
