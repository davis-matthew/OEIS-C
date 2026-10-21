/*
 * A086298  Numbers n such that 1-2n+3n^2 is prime.
 * https://oeis.org/A086298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m such that 1 - 2m + 3m² is prime
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2)               return false;
    if (n % 2 == 0)          return n == 2;
    if (n % 3 == 0)          return n == 3;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases valid for all 64‑bit numbers. */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, p = d, b = a;
        while (p) {                     /* a^d mod n */
            if (p & 1) x = (x * b) % n;
            b = (b * b) % n;
            p >>= 1;
        }

        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cache of values that satisfy the property. */
static seq_t *cached   = NULL;   /* sequence values */
static size_t cache_size = 0;   /* number of cached elements */
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t  next_candidate = 1; /* smallest value not yet tested */

/* Return the n‑th integer m such that 1 – 2m + 3m² is prime. */
seq_t a(idx_t n)
{
    /* Expand the cache until it contains at least n elements. */
    while (cache_size < n) {
        /* Compute f = 1 - 2k + 3k² using 128‑bit arithmetic for safety. */
        __int128 k  = next_candidate;
        __int128 f1 = (__int128)1 - (__int128)2 * k + (__int128)3 * k * k;

        if (f1 > 1) {
            uint64_t f = (uint64_t)f1;           /* fits into 64 bits for the
                                                    relevant range of k. */
            if (is_prime_64(f)) {
                /* Grow the cache if necessary. */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cached,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();            /* allocation failure */
                    cached = tmp;
                    cache_cap = new_cap;
                }
                cached[cache_size++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
    return cached[n - 1];
}
