/*
 * A082491  a(n) = n! * d(n), where n! = factorial numbers (A000142), d(n) =
 *          subfactorial numbers (A000166).
 * https://oeis.org/A082491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*a(n-1) + (-1)^n * n!; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Static caches to store factorials and sequence terms */
static seq_t *fact_cache = NULL;
static seq_t *a_cache   = NULL;
static size_t fact_cap  = 0;
static size_t a_cap     = 0;
static idx_t fact_len   = 0;   /* number of factorials computed (fact_len>0 means fact[0..fact_len-1] ready) */
static idx_t a_len      = 0;   /* number of a values computed (a_len>0 means a[0..a_len-1] ready) */

/* Compute A082491 value for given n */
seq_t a(idx_t n)
{
    /* Ensure the cache arrays are large enough to hold indices up to n */
    ensure_cache(n, &fact_cache, &fact_cap);
    ensure_cache(n, &a_cache, &a_cap);

    /* Compute factorials up to n if not already done */
    if (fact_len == 0) {
        fact_cache[0] = (seq_t)1;
        fact_len = 1;
    }
    for (idx_t i = fact_len; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_len = n + 1;   /* we now have fact[0..n] */

    /* Compute sequence values up to n using the recurrence */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i * a_cache[i - 1];
        if ((i & 1) == 0)   /* even i: (+1) term */
            term += fact_cache[i];
        else                 /* odd i: (-1) term */
            term -= fact_cache[i];
        a_cache[i] = term;
    }
    a_len = n + 1;   /* we now have a[0..n] */

    return a_cache[n];
}
