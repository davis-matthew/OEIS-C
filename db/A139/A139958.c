/*
 * A139958  Primes of the form 8x^2+51y^2.
 * https://oeis.org/A139958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 8*x^2 + 51*y^2 The function returns the n‑th
 *            prime that can be represented as 8x²+51y² (OEIS offset 1). It
 *            uses a simple search with a prime test from the provided
 *            utilities
 * Range:     u8 n <= 4, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Residues modulo 408 that primes of this form must have. */
static const unsigned residues[8] = {35, 59, 83, 155, 179, 203, 251, 395};

/* Integer square root of a 64‑bit unsigned integer, implemented without
   floating point. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* Test whether a 64‑bit integer is a perfect square. */
static bool is_square_uint64(uint64_t x)
{
    uint64_t r = isqrt_u64(x);
    return r * r == x;
}

/* Check that the prime lies in one of the residues modulo 408. */
static bool is_residue_prime(uint64_t p)
{
    uint64_t r = p % 408;
    for (int i = 0; i < 8; ++i)
        if (r == residues[i])
            return true;
    return false;
}

/* Try all possible x to see if p can be written as 8x² + 51y². */
static bool has_representation(uint64_t p)
{
    uint64_t max_x = isqrt_u64(p / 8);
    for (uint64_t x = 0; x <= max_x; ++x) {
        uint64_t lhs = 8 * x * x;
        if (lhs > p)
            break;
        uint64_t rem = p - lhs;
        if (rem % 51)
            continue;
        uint64_t y2 = rem / 51;
        if (is_square_uint64(y2))
            return true;
    }
    return false;
}

/* Cache of already found primes of the required form. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

/* Next odd candidate to test. */
static uint64_t candidate = 3;

/* Grow the cache to at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr)
        abort();    /* unrecoverable */
    cache = newptr;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1 */

    /* Find enough terms. */
    while (cache_len < n) {
        /* Find the next prime of the required form. */
        while (true) {
            uint64_t p = candidate;
            candidate += 2; /* keep candidate odd */

            if (!is_prime((idx_t)p))
                continue;
            if (!is_residue_prime(p))
                continue;
            if (!has_representation(p))
                continue;

            /* Store the found prime. */
            grow_cache(cache_len + 1);
            cache[cache_len] = (seq_t)p;
            ++cache_len;
            break;
        }
    }

    return cache[n - 1];
}
