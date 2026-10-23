/*
 * A042153  Denominators of continued fraction convergents to sqrt(601).
 * https://oeis.org/A042153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = cf[(n-1) mod L] * a(n-1) + a(n-2),
 *  with a(0)=1, a(1)=cf[0] (cf[0] = 1).
 *  cf is the list of partial quotients of the periodic continued
 *  fraction for √601; L = 31.  The recurrence is equivalent to the
 *  OEIS recurrence a(n)=278936607359064*a(n-31)+a(n-62).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to compute the coefficients of the continued fraction of
 * sqrt(601).  The period of the continued fraction is stored in
 * cf_period.  */
static seq_t cf_period[128];
static size_t cf_period_len = 0;

static void init_cf_period(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    const uint64_t D = 601;
    /* Compute floor(sqrt(D)). */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    for (;;)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        cf_period[cf_period_len++] = (seq_t)a;
        if (a == 2 * a0) break;          /* period ends when a == 2*a0 */
    }
}

/* Dynamic cache for previously computed denominators.  The cache
   stores values for indices 0 .. cache_cap-1.  */
static seq_t *denom_cache    = NULL;
static size_t cache_cap      = 0;
static size_t max_cached     = 0;   /* largest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to n.  The function returns
   the (possibly reallocated) cache pointer.  */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(denom_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* Zero‑initialize the new part of the cache. */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    denom_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th denominator of the continued fraction convergents
 * to √601.  The function works for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_cf_period();
    ensure_cache_cap(n);

    /* Initialise the base cases if they have not been set yet. */
    if (max_cached == 0)
    {
        denom_cache[0] = (seq_t)1;
        denom_cache[1] = cf_period[0];
        max_cached = 1;
    }

    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t coeff = cf_period[(i - 1) % cf_period_len];
        denom_cache[(size_t)i] = coeff * denom_cache[(size_t)(i - 1)]
                                + denom_cache[(size_t)(i - 2)];
    }
    max_cached = (size_t)n;
    return denom_cache[(size_t)n];
}
