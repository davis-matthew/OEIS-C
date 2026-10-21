/*
 * A074179  a(1) = 1, a(n) = smallest multiple of n divisible by the sum of
 *          all previous terms.
 * https://oeis.org/A074179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, Sum_{k=1..n-1} a(k)), a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

/* Cached sequence values and cumulative sums */
static seq_t *a_cache    = NULL;
static seq_t *cum_cache  = NULL;
static size_t a_cap      = 0;
static size_t cum_cap    = 0;
static idx_t  max_cached = 0;

/* Ensure the caches can hold at least n+1 entries (indices 0..n) */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 > a_cap) {
        a_cache = seq_realloc(a_cache, a_cap, (size_t)n + 1);
        a_cap   = (size_t)n + 1;
    }
    if ((size_t)n + 1 > cum_cap) {
        cum_cache = seq_realloc(cum_cache, cum_cap, (size_t)n + 1);
        cum_cap   = (size_t)n + 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) /* Undefined for n < 1, but return 0 for safety */
        return 0;

    /* Initialize caches on first call */
    if (max_cached == 0) {
        /* Allocate space for indices 0 and 1 */
        ensure_cache_capacity(1);
        cum_cache[0] = (seq_t)0;   /* sum of zero terms */
        a_cache[0]   = (seq_t)0;   /* unused, but set to 0 */
        a_cache[1]   = (seq_t)1;   /* a(1) */
        cum_cache[1] = (seq_t)1;   /* cumulative sum up to 1 */
        max_cached   = 1;
    }

    /* If we already have the requested term, return it */
    if ((size_t)n <= max_cached)
        return a_cache[n];

    /* Ensure capacity for all entries up to n */
    ensure_cache_capacity(n);

    /* Compute missing terms */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t sum_prev = cum_cache[i - 1];
        seq_t i_seq    = (seq_t)i;

        /* lcm(i, sum_prev) = i * (sum_prev / gcd(i, sum_prev)) */
        seq_t g        = gcd(i_seq, sum_prev);
        seq_t a_val    = (sum_prev / g) * i_seq;

        a_cache[i]     = a_val;
        cum_cache[i]   = sum_prev + a_val;
    }

    max_cached = n;
    return a_cache[n];
}
