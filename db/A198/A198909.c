/*
 * A198909  Number of nX3 0..7 arrays with values 0..7 introduced in row major
 *          order and no element equal to any horizontal or vertical neighbor.
 * https://oeis.org/A198909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A198909. a(n) = 431*a(n-1)
 *            -53038*a(n-2) +2570412*a(n-3) -55789402*a(n-4) +610919758*a(n-5)
 *            -3707175288*a(n-6) +13010264300*a(n-7) -26380804149*a(n-8)
 *            +29517406483*a(n-9) -16243626186*a(n-10) +3246286680*a(n-11)
 *            Initial values: a(1)=2, a(2)=34, a(3)=2051, a(4)=269940, a(5...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const long long coeff[11] = {
    431LL, -53038LL, 2570412LL, -55789402LL,
    610919758LL, -3707175288LL, 13010264300LL, -26380804149LL,
    29517406483LL, -16243626186LL, 3246286680LL
};

static const char *init_str[11] = {
    "2",
    "34",
    "2051",
    "269940",
    "54381563",
    "13088156547",
    "3352514013159",
    "876632051686733",
    "230783525290600476",
    "60892298359864756053",
    "16078352442288906898825"
};

static seq_t *cache = NULL;          /* cache[n] holds a(n), n>=1 */
static idx_t  cache_cap = 0;          /* allocated size (index 0 unused) */
static idx_t  cache_max = 0;          /* largest n stored */

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = n + 1;               /* include index 0, unused */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    if (new_cache != cache) {
        cache = new_cache;
        /* Initialise new entries to zero. */
        for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    }
    cache_cap = new_cap;
}

/* Store initial values in cache. */
static void initialize_cache(void)
{
    if (cache != NULL) return;
    ensure_cache_cap(11);
    for (idx_t i = 1; i <= 11; ++i)
        cache[i] = seq_from_str(init_str[i-1]);
    cache_max = 11;
}

/* Compute values up to n (if not already). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;
    if (n <= 11) { /* should already be initialised */
        cache_max = n;
        return;
    }
    ensure_cache_cap(n);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0; /* sum of positive terms */
        seq_t neg = (seq_t)0; /* sum of negative terms (to be subtracted) */
        for (int k = 0; k < 11; ++k) {
            seq_t term = cache[i - (k + 1)] * (seq_t)llabs(coeff[k]);
            if (coeff[k] >= 0)
                pos += term;
            else
                neg += term;
        }
        /* Final value is pos - neg; the recurrence guarantees non‑negative result. */
        cache[i] = pos - neg;
    }
    cache_max = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* Out of bounds, undefined */
    initialize_cache();
    compute_up_to(n);
    return cache[n];
}
