/*
 * A174830  Odd numbers k such that k^2 is an abundant number.
 * https://oeis.org/A174830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer k such that k^2 is an abundant
 *            number. Computation: iterate over odd integers k, test if k² is
 *            abundant (σ(k²) > 2·k²) using prime factorizations supplied by
 *            `factor_u64`. The discovered terms are cached so that calls with
 *            arbitrary order are efficient
 * Range:     u8 none, u16 n <= 1, u32 n <= 356, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helpers provided by the build system */
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array(size_t) */
#include "../utils/seq_realloc.h"       /* seq_realloc(seq_t*,size_t,size_t) */

/* --- caching machinery --------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of found odd k values          */
static size_t cache_len  = 0;     /* number of terms actually stored      */
static size_t cache_cap  = 0;     /* current allocated capacity           */
static uint64_t next_k   = 1;     /* next odd integer to examine         */

/* Expand the cache array if needed.  The function is safe for both
   built‑in integer types and arbitrary‑precision types because it
   uses the provided seq_realloc helper. */
static void ensure_cache_space(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        /* seq_realloc returns NULL on failure; we ignore errors
           because the test harness guarantees enough memory. */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* --- helper: is k² abundant? ------------------------------------------- */
static bool abundant_square(uint64_t k)
{
    /* Factor k.  The helper fills the arrays with prime factors
       and their exponents.  The array size 64 is safe because
       a 64‑bit number has at most 16 distinct prime factors.   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;

    factor_u64(k, primes, exps, &cnt);

    /* Compute σ(k²) as the product over primes
       σ(p^(2e)) = 1 + p + ... + p^(2e).                     */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = 1;
        seq_t cur  = 1;
        uint64_t p = primes[i];
        uint64_t exp = exps[i];
        uint64_t e2  = exp * 2;          /* exponent in k²          */

        for (uint64_t j = 1; j <= e2; ++j) {
            cur  = cur * (seq_t)p;
            term = term + cur;
        }
        sigma = sigma * term;
    }

    /* Compute 2·k² in seq_t. */
    seq_t rhs = 2;
    rhs = rhs * (seq_t)k;
    rhs = rhs * (seq_t)k;

    return sigma > rhs;   /* abundant ⇔ σ(n) > 2n */
}

/* --- main sequence accessor ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1; harmless */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next odd integer with an abundant square. */
        while (true) {
            if (abundant_square(next_k)) {
                ensure_cache_space();
                cache[cache_len++] = (seq_t)next_k;
                next_k += 2;           /* skip to the following odd */
                break;
            }
            next_k += 2;               /* try next odd number      */
        }
    }
    return cache[(size_t)n - 1];
}
