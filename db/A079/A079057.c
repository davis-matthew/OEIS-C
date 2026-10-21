/*
 * A079057  a(n) = Sum_{k=1..n} bigomega(tau(k)).
 * https://oeis.org/A079057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} Ω( τ(k) ) (OEIS A079057)
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* for uint32_t */
#include <stdlib.h>      /* for calloc, realloc */
#include "../utils/bigomega.h"   /* provides seq_t bigomega(idx_t) */

/* cached data */
static uint32_t *tau_cache   = NULL;  /* divisor count up to n */
static seq_t   *cum_cache   = NULL;  /* cumulative sums up to n */
static idx_t    cached_upto = 0;     /* largest n that has been cached */

/* recompute tau and cumulative sums up to n (inclusive) */
static void recompute_until(idx_t n)
{
    /* allocate arrays of size n+1 (index 0 unused for tau, but needed for cum_cache) */
    size_t size = (size_t)n + 1;

    /* allocate or reallocate tau_cache */
    {
        uint32_t *new_tau = (uint32_t *)realloc(tau_cache, size * sizeof(uint32_t));
        if (!new_tau) return;          /* on allocation failure, leave cache unchanged */
        tau_cache = new_tau;
    }

    /* allocate or reallocate cum_cache */
    {
        seq_t *new_cum = (seq_t *)realloc(cum_cache, size * sizeof(seq_t));
        if (!new_cum) return;
        cum_cache = new_cum;
    }

    /* initialise tau counts to zero */
    for (idx_t i = 1; i <= n; ++i) tau_cache[i] = 0;

    /* sieve to compute divisor counts */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++tau_cache[j];
        }
    }

    /* compute cumulative sums */
    cum_cache[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        cum_cache[i] = cum_cache[i-1] + bigomega((idx_t)tau_cache[i]);
    }

    cached_upto = n;
}

/* sequence function: a(n) = Σ_{k=1}^n Ω( τ(k) ) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; n=0 is undefined */

    if (n > cached_upto) {
        recompute_until(n);
    }
    return cum_cache[n];
}
