/*
 * A166700  Totally multiplicative sequence with a(p) = a(p-1) + 3 for prime
 *          p.
 * https://oeis.org/A166700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = ∏p^e‖n (a(p-1)+3)^e (n≥1) Multiplicative with a(1)=1,
 *            a(p^e) = (a(p-1)+3)^e
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/*  Dependencies:                                                  */
/*    - factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
/*    - all other helpers are assumed available in the build        */
/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* declares factor_u64() */

/* cache for already computed values (1‑based index) */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache contains entries 1..n, initializing new entries to 0. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;

    idx_t new_cap = n;
    if (new_cap < 10)          /* small start size */
        new_cap = 10;
    else if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;   /* grow roughly doubling */

    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();   /* unrecoverable allocation failure */

    /* initialise new entries (old ones keep their values) */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache      = new_cache;
    cache_cap  = new_cap;

    if (cache[1] == (seq_t)0)       /* base case */
        cache[1] = (seq_t)1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)         /* unused offset; safe guard */
        return (seq_t)0;

    ensure_cache(n);

    /* return memoised value if present */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* base value */
    if (n == 1) {
        cache[1] = (seq_t)1;
        return (seq_t)1;
    }

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);   /* helper function */

    seq_t result = (seq_t)1;

    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = primes[k];
        size_t   e = (size_t)exps[k];

        /* compute a(p-1) + 3 */
        seq_t base = a(p - 1) + (seq_t)3;

        /* compute (base)^e by repeated multiplication */
        seq_t term = (seq_t)1;
        for (size_t j = 0; j < e; ++j)
            term *= base;

        result *= term;
    }

    cache[n] = result;
    return result;
}
