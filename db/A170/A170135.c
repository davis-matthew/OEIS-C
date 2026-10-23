/*
 * A170135  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 ≤ n ≤ 37: a(n) = 28 * Σ_{k=0}^{n-1} a(k) + 2;
 *            for n = 38: a(n) = 28 * Σ_{k=1}^{37} a(k) - 406 * a(0) + 1; for
 *            n > 38: a(n) = 28 * Σ_{k=n-37}^{n-1} a(k) - 406 * a(n-38);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached sequence values */
static seq_t *seq_vals = NULL;
/* Prefix sums of the sequence */
static seq_t *seq_pref = NULL;
/* Number of computed terms (next index to compute is cached_len) */
static idx_t cached_len = 0;
/* Allocated capacity of the arrays */
static size_t cached_cap = 0;

/* Ensure that terms up to index n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (cached_len > n) return;           /* already computed */

    /* Extend allocation if necessary */
    size_t needed = (size_t)n + 1;
    if (cached_cap < needed) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_t *tmp_pref = (seq_t *)realloc(seq_pref, new_cap * sizeof(seq_t));
        if (!tmp_vals || !tmp_pref) abort();                 /* allocation failed */
        seq_vals = tmp_vals;
        seq_pref = tmp_pref;
        cached_cap = new_cap;
    }

    /* Initialise the very first term if the cache is empty */
    if (cached_len == 0) {
        seq_vals[0] = 1;
        seq_pref[0] = 1;
        cached_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        size_t ii = (size_t)i;                    /* cast to size_t for indexing */
        seq_t sum;
        if (i <= 37) {
            sum = seq_pref[ii - 1];               /* Σ_{k=0}^{i-1} a(k) */
        } else {
            sum = seq_pref[ii - 1] - seq_pref[ii - 38]; /* Σ_{k=i-37}^{i-1} a(k) */
        }

        seq_t term;
        if (i <= 37) {
            term = 28 * sum + 2;
        } else if (i == 38) {
            term = 28 * sum - 406 * seq_vals[ii - 38] + 1;
        } else {
            term = 28 * sum - 406 * seq_vals[ii - 38];
        }

        seq_vals[ii] = term;
        seq_pref[ii] = seq_pref[ii - 1] + term;
    }

    cached_len = n + 1;
}

/* Exported function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_vals[n];
}
