/*
 * A236861  Characteristic function of A236841 (A234741): a(n) = 1 if n is a
 *          result of downward remultiplication (N -> GF(2)[X]) of some
 *          number, 0 otherwise.
 * https://oeis.org/A236861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is 1 if n is 0, 1, a prime, or can be expressed as * a
 *            carry‑less product (GF(2)[x]) of two numbers k,m < n with *
 *            a(k)=a(m)=1. Composite numbers that are irreducible polynomials
 *            * over GF(2) are 0. Products are computed as carry‑less
 *            multiplication * of the binary polynomials corresponding...
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   mul_carryless_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_carryless_uint64.h"

/* --------------------------------------------------------------------- */
/* Helper functions ----------------------------------------------------- */

/* Return true if f is divisible by g in GF(2)[x] (carry‑less division). */
static bool poly_divides(uint64_t f, uint64_t g)
{
    if (g == 0)
        return false;
    int dg = 63 - __builtin_clzll(g);
    uint64_t r = f;
    while (r != 0) {
        int dr = 63 - __builtin_clzll(r);
        if (dr < dg)
            break;
        r ^= g << (dr - dg);
    }
    return r == 0;
}

/* Test whether a composite integer >1 represents an irreducible polynomial
 * over GF(2).  For small values this brute‑force method is fast enough.
 */
static bool is_irreducible_uint64(uint64_t n)
{
    if (n <= 1)
        return false;                 /* not used for primes */
    int deg = 63 - __builtin_clzll(n);
    int half = deg / 2;
    for (int dg = 1; dg <= half; ++dg) {
        uint64_t min_g = 1ULL << dg;
        uint64_t max_g = (1ULL << (dg + 1)) - 1;
        for (uint64_t g = min_g; g <= max_g; ++g) {
            if (poly_divides(n, g))
                return false;          /* reducible */
        }
    }
    return true;                       /* irreducible */
}

/* --------------------------------------------------------------------- */
/* Cached computation --------------------------------------------------- */
static seq_t *cache = NULL;          /* a(n) values */
static idx_t cache_len = 0;          /* size of cache (next free index) */
static uint8_t *reach = NULL;        /* boolean reachable via product */
static idx_t reach_len = 0;          /* size of reach array */
static uint8_t *prime = NULL;        /* prime sieve flag */
static idx_t prime_len = 0;          /* size of prime array */

static idx_t *ones = NULL;           /* list of numbers with a = 1 and >1 */
static idx_t ones_count = 0;
static idx_t ones_cap = 0;

/* Rebuild the cache up to include all indices <= n. */
static bool rebuild(idx_t n)
{
    /* Resize cache, reach, prime arrays */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        return false;
    cache = new_cache;

    uint8_t *new_reach = (uint8_t *)realloc(reach, new_len);
    if (new_reach == NULL)
        return false;
    reach = new_reach;

    uint8_t *new_prime = (uint8_t *)realloc(prime, new_len);
    if (new_prime == NULL)
        return false;
    prime = new_prime;

    /* Sieve of Eratosthenes for primes up to n. */
    for (idx_t i = 0; i < new_len; ++i)
        prime[i] = 1;
    if (new_len > 0) prime[0] = 0;
    if (new_len > 1) prime[1] = 0;
    for (idx_t p = 2; (idx_t)p * p <= n; ++p) {
        if (prime[p]) {
            for (idx_t q = p * p; q <= n; q += p)
                prime[q] = 0;
        }
    }

    /* Reinitialize reach array */
    for (idx_t i = 0; i < new_len; ++i)
        reach[i] = 0;

    /* Resize list of known "1" numbers, >1 */
    if (new_len > ones_cap) {
        idx_t *new_ones = (idx_t *)realloc(ones, new_len * sizeof(idx_t));
        if (new_ones == NULL)
            return false;
        ones = new_ones;
        ones_cap = new_len;
    }
    ones_count = 0;

    /* Compute a(i) for all i <= n */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t val;
        if (i == 0 || i == 1) {
            val = (seq_t)1;
        } else if (prime[i]) {
            val = (seq_t)1;
        } else if (is_irreducible_uint64(i)) {
            val = (seq_t)0;
        } else {
            /* Reducible composite */
            val = reach[i] ? (seq_t)1 : (seq_t)0;
        }
        cache[i] = val;

        if (i > 1 && val == (seq_t)1) {
            /* Generate products of this number with all existing >1 ones */
            idx_t prev = ones_count;
            for (idx_t j = 0; j < prev; ++j) {
                uint64_t x = ones[j];
                uint64_t p = mul_carryless_uint64((uint64_t)i, x);
                if ((idx_t)p <= n)
                    reach[(idx_t)p] = 1;
            }
            /* Self‑product */
            uint64_t p = mul_carryless_uint64((uint64_t)i, (uint64_t)i);
            if ((idx_t)p <= n)
                reach[(idx_t)p] = 1;

            /* Store in list */
            ones[ones_count++] = i;
        }
    }

    cache_len = new_len;
    reach_len = new_len;
    prime_len = new_len;
    return true;
}

seq_t a(idx_t n)
{
    if (n + 1 > cache_len) {
        if (!rebuild(n))
            return (seq_t)0;          /* out of memory or other failure */
    }
    return cache[n];
}
