/*
 * A106932  Primes of the form x^2 + xy + 17y^2, with x and y nonnegative.
 * https://oeis.org/A106932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime that can be written as x^2 + x*y + 17*y^2
 *            with non‑negative integers x,y. The values are generated in
 *            increasing order by testing successive primes. The computation
 *            uses a fast integer square‑root for 128‑bit arguments and the
 *            is_prime routine from ../utils/is_prime.h
 * Range:     u8 n <= 27, u16 n <= 2974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Integer square‑root for unsigned __int128 (returns a 64‑bit value) */
static unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned long long low = 0;
    unsigned long long high = (1ULL << 32);           /* √(2^65−1) < 2^32+1 */
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether a prime p can be represented as x^2 + x*y + 17*y^2
   with non‑negative integers x,y.  The test examines the Diophantine
   equation derived from the quadratic form: 4p = (2x+y)^2 + 67y^2. */
static bool has_representation(uint64_t p)
{
    /* Only examine y such that 67*y^2 <= 4p */
    for (uint64_t y = 0; ; ++y) {
        unsigned __int128 lhs = (unsigned __int128)4 * p;
        unsigned __int128 rhs = (unsigned __int128)67 * y * y;
        if (rhs > lhs)
            break;                                   /* no further solutions */
        unsigned __int128 D = lhs - rhs;              /* discriminant */
        unsigned __int128 s = isqrt128(D);
        if (s * s != D)
            continue;                                 /* not a perfect square */
        /* (s - y) must be even and non‑negative so that x = (s - y)/2 is integer ≥0 */
        if (s >= y && ((s - y) & 1ULL) == 0)
            return true;
    }
    return false;
}

/* Global state for memoisation */
static seq_t *cache = NULL;          /* array of found terms */
static idx_t   cache_size = 0;       /* number of stored terms */
static idx_t   cache_cap  = 0;       /* allocated capacity */
static uint64_t next_prime_candidate = 2;   /* smallest prime not yet examined */

/* The required sequence function */
seq_t a(idx_t n)
{
    /* Grow the cache array if necessary */
    if (cache_size < n) {
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_size + (n - cache_size) > cache_cap) {
            while (cache_cap < cache_size + (n - cache_size))
                cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }

    /* Generate terms until we have the n‑th */
    while (cache_size < n) {
        /* Use the provided primality test */
        if (is_prime(next_prime_candidate)) {
            if (has_representation(next_prime_candidate))
                cache[cache_size++] = (seq_t)next_prime_candidate;
        }
        if (next_prime_candidate == UINT64_MAX)      /* safety */
            break;
        ++next_prime_candidate;
    }
    return cache[n - 1];
}
