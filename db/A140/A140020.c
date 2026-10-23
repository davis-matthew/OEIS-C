/*
 * A140020  Primes of the form 13x^2+105y^2.
 * https://oeis.org/A140020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 13*x^2 + 105*y^2 by inspection of
   successive primes.  The sieve filter uses the 37 admissible residues
   modulo the discriminant 5460.  */

/* Include the delegated utilities. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

/* The discriminant is 5460.  Primes of the form 13*x^2+105*y^2 must
   lie in one of these residue classes.  */
static const uint16_t residues[37] = {
    13, 157, 313, 433, 493, 517, 913, 937, 997, 1153, 1273,
    1693, 1777, 1837, 1993, 2077, 2497, 2617, 2677, 2833,
    3013, 3097, 3253, 3337, 3433, 3457, 3793, 3877, 4177,
    4213, 4273, 5017, 5053, 5113, 5197, 5353, 5437
};

/* Helper table to rapidly reject primes that cannot be of the required form. */
static bool mod_ok[5460];
static bool mod_ok_initialized = false;
static void init_mod_ok(void)
{
    for (int i = 0; i < 5460; ++i) mod_ok[i] = false;
    for (int i = 0; i < 37; ++i) mod_ok[residues[i]] = true;
    mod_ok_initialized = true;
}

/* Integer square‑root (floor) for 64‑bit values. */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

/* Test whether an odd prime p can be expressed as 13*x^2 + 105*y^2. */
static bool has_representation(uint64_t p)
{
    /* 105*y^2 must be <= p.  The loop stops when this condition fails. */
    for (uint64_t y = 0; ; ++y) {
        __uint128_t yy = (__uint128_t)y * y;
        __uint128_t term = (__uint128_t)105 * yy;
        if (term > p) break;
        uint64_t remainder = p - (uint64_t)term;  /* remainder is <= 2^64-1 */
        if (remainder % 13 != 0) continue;
        uint64_t x2 = remainder / 13;
        uint64_t x = isqrt_uint64(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* Cache for generated terms (0‑based). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked_prime = 1;   /* prime to start searching from */

/* Append a new term to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1; return 0 */

    if (!mod_ok_initialized) init_mod_ok();

    /* Generate primes until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        uint64_t prime = next_prime(last_checked_prime + 1);
        last_checked_prime = prime;

        /* Quick reject using the residues modulo 5460. */
        if (!mod_ok[prime % 5460]) continue;

        /* Exact check for the quadratic form representation. */
        if (has_representation(prime))
            cache_append((seq_t)prime);
    }

    return cache[n - 1];
}
