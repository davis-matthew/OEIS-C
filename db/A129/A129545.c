/*
 * A129545  Triangular numbers T such that T+1 is a prime.
 * https://oeis.org/A129545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 79, u32 n <= 9219, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A129545 – triangular numbers T such that T+1 is prime.
   The function finds the first n such triangular numbers by
   incrementally testing T_k+1 for primality. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        unsigned __int128 x = 1, power = a;
        uint64_t d_curr = d;
        while (d_curr) {
            if (d_curr & 1ULL) x = (x * power) % n;
            power = (power * power) % n;
            d_curr >>= 1;
        }
        uint64_t xi = (uint64_t)x;
        if (xi == 1 || xi == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            xi = (uint64_t)x;
            if (xi == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Cached results of the sequence. */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_cap = 0;      /* allocated capacity */
static idx_t  cache_len = 0;      /* number of cached terms */
static uint64_t next_k = 1;       /* next triangular index to test */

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used; defensive */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Compute terms until we have the required n‑th one. */
    while (cache_len < n) {
        unsigned __int128 k = next_k;
        unsigned __int128 T = k * (k + 1) / 2;     /* triangular number */
        unsigned __int128 Tp1 = T + 1;             /* T + 1 */
        uint64_t candidate = (uint64_t)Tp1;        /* fits 64‑bit for our range */

        if (is_prime_uint64(candidate)) {
            if (cache_len == cache_cap) {         /* grow cache if needed */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)T;           /* store value */
        }
        ++next_k;
    }

    return cache[n - 1];
}
