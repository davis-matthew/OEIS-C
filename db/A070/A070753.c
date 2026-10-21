/*
 * A070753  Primes p such that sin(p) < 0.
 * https://oeis.org/A070753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that sin(p) < 0 (p in radians)
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code generates primes incrementally, tests the sign of sin(p), and
   caches the ones with negative sine.  It uses a simple deterministic
   primality test suitable for the ranges needed for n up to ~10 000. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/* --- small deterministic primality test -------------------------------- */
// Works for all 64‑bit unsigned integers (fast enough for the required range).
static bool is_prime_uint64(uint64_t n) {
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)   return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- cache of primes with sin(p) < 0 --------------------------------- */
static seq_t *cache = NULL;      /* cached terms */
static idx_t  cache_cnt = 0;     /* number of cached terms    */
static idx_t  cache_cap = 0;     /* allocated capacity        */
static uint64_t next_candidate = 2;   /* next integer to test      */

static void append_cache(uint64_t v)
{
    if (cache_cnt == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
    cache[cache_cnt++] = (seq_t)v;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    while (cache_cnt < n) {
        /* find the next prime */
        while (! is_prime_uint64(next_candidate))
            ++next_candidate;

        /* test the sign of sin(p) */
        if (sin((double)next_candidate) < 0.0)
            append_cache(next_candidate);

        ++next_candidate;
    }
    return cache[n - 1];
}
