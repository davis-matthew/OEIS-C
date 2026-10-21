/*
 * A213050  Primes of the form 4*k+1 with primitive root +2.
 * https://oeis.org/A213050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that both +2 and –2 are primitive
 *            roots modulo p. The sequence is computed by iterating through
 *            primes, testing the primitive root condition, and caching the
 *            results so that subsequent calls can be answered in O(1)
 * Range:     u8 n <= 11, u16 n <= 1230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* External helper prototypes */
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Cache for sequence entries */
static seq_t *cache      = NULL;  /* dynamic array of seq_t values */
static size_t cache_len  = 0;     /* number of stored terms          */
static size_t cache_cap  = 0;     /* allocated capacity               */

/* Append a new element to the cache, reallocating if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) return;  /* allocation failed – silently abort */
        cache   = new_buf;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Test whether g is a primitive root modulo prime p (p > 2).  Works for
   both 64‑bit and arbitrary‑precision seq_t since it uses the helper
   mod_pow that accepts seq_t arguments. */
static bool is_primitive_root_uint64(uint64_t g, uint64_t p)
{
    uint64_t m = p - 1ULL;

    /* Factor m using the fast 64‑bit factorizer. */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt_f = 0;
    factor_u64(m, primes_f, exps_f, &cnt_f);

    for (size_t i = 0; i < cnt_f; ++i) {
        uint64_t q   = primes_f[i];
        uint64_t exp = m / q;
        seq_t res = mod_pow((seq_t)(g % p), exp, (seq_t)p);
        if (res == (seq_t)1)   /* order divides m/q → not primitive root */
            return false;
    }
    return true;                /* full order p‑1 found */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, return 0 as safety */

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Start searching for new terms. */
    uint64_t cur = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] : 3ULL;

    while (cache_len < (size_t)n) {
        cur = next_prime(cur);              /* next prime > cur */
        if ((cur & 3ULL) != 1) continue;    /* not of the form 4k+1 */

        if (is_primitive_root_uint64(2ULL, cur) &&
            is_primitive_root_uint64(cur - 2ULL, cur))
        {
            append_cache((seq_t)cur);
        }
    }

    return cache[(size_t)n - 1];
}
