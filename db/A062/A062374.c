/*
 * A062374  Euler phi(n) / Carmichael lambda(n) = 4.
 * https://oeis.org/A062374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   factor_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS formula for A062374: the n‑th integer m such that
   φ(m)/λ(m) = 4.  The function a(idx_t n) returns a(1)=24, a(2)=40, … 
   by searching for values in ascending order and caching the results. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/phi.h"          /* seq_t phi(idx_t m);            */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b);   */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, ...); */

/* Maximum number of terms we are prepared to cache (sufficient for n≈10000). */
#define MAX_CACHED_TERMS 20000

/* Cache for found terms.  The array is large enough for the expected use
   and avoids dynamic reallocation that would fail for arbitrary‑precision
   types. */
static seq_t cache[MAX_CACHED_TERMS];
static size_t cache_len = 0;

/* Last integer checked when building the cache. */
static uint64_t search_limit = 0;

/* Return the Carmichael lambda of a 64‑bit integer n as a seq_t. */
static seq_t lambda_u64(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t lcm_val = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part = 0;

        if (p == 2) {
            if (e == 1)            part = 1;
            else if (e == 2)       part = 2;
            else {
                part = 1;
                for (uint64_t j = 0; j < e - 2; ++j)
                    part *= 2;
            }
        } else {
            /* part = p^(e-1) * (p-1) */
            part = 1;
            for (uint64_t j = 0; j < e - 1; ++j)
                part *= (seq_t)p;
            part *= (seq_t)(p - 1);
        }

        /* lcm(lcm_val, part) = lcm_val / gcd(lcm_val, part) * part */
        seq_t g = gcd(lcm_val, part);
        lcm_val = lcm_val / g * part;
    }
    return lcm_val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined, but harmless */

    /* Build the cache until we have the nth term. */
    while (cache_len < n) {
        ++search_limit;             /* next integer to test */

        seq_t phi_val   = phi((idx_t)search_limit);
        seq_t lambda_val= lambda_u64(search_limit);

        if (phi_val == 4 * lambda_val) {
            if (cache_len < MAX_CACHED_TERMS)
                cache[cache_len++] = (seq_t)search_limit;
        }
    }

    return cache[n - 1];
}
