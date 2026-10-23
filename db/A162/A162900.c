/*
 * A162900  a(1)=1. For n >= 2, a(n) is the smallest integer > a(n-1) such
 *          that gcd(n, a(n)) = p^k, where p is a prime, k >= 1.
 * https://oeis.org/A162900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A162900:
   a(1)=1; for n>=2, a(n) is the smallest integer > a(n‑1)
   such that gcd(n, a(n)) is a prime power (>=2). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* cache[1] holds a(1) */
static size_t cache_cap = 0;         /* current allocated capacity (in terms) */
static size_t cache_last = 0;        /* number of terms computed so far */

/* Ensure the cache can hold indices up to n (inclusive).  Uses standard
   realloc; multiplication by sizeof(seq_t) keeps the size correct for
   any integer type. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Test whether x (>1) is a prime power p^k (k ≥ 1). */
static bool is_prime_power(uint64_t x)
{
    if (x <= 1) return false;
    int distinct = 0;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            ++distinct;
            if (distinct > 1) return false;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++distinct;   /* leftover prime factor */
    return distinct == 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for underflow */

    ensure_capacity(n);

    /* Initialise the first term if not done yet. */
    if (cache_last == 0) {
        cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Compute missing terms up to n. */
    while (cache_last < (size_t)n) {
        idx_t i = (idx_t)(cache_last + 1);
        seq_t prev = cache[cache_last];
        seq_t cand = prev + 1;

        while (1) {
            seq_t g = gcd((seq_t)i, cand);
            if (g > (seq_t)1 && is_prime_power((uint64_t)g)) {
                cache[cache_last + 1] = cand;
                break;
            }
            ++cand;
        }
        ++cache_last;
    }

    return cache[(size_t)n];
}
