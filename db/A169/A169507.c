/*
 * A169507  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence of order 34 derived from the generating function *
 *            a(0) = 1 * For 0 < n < 34: a(n) = p_n + 12 * Σ_{k=0}^{n-1} a(k)
 *            with p_n = 2 * For n = 34: a(34) = 12 * Σ_{k=1}^{33} a(k) - 78 *
 *            a(0) + 1 * For n > 34: a(n) = 12 * Σ_{k=n-33}^{n-1} a(k) - 78 *
 *            a(n-34) * * All arithmetic is performed in...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms  */
static size_t cache_cap = 0;         /* allocated capacity      */
static idx_t computed_last = (idx_t)-1;/* largest index computed  */

/* running sums used during computation */
static seq_t global_sum = (seq_t)0;   /* Σ_{k=0}^{i} a(k) for i = computed_last */
static seq_t window_sum = (seq_t)0;   /* Σ_{k=i-33}^{i-1} a(k)  for current i */

/* Ensure that cache can hold indices up to 'need-1' */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? (cache_cap * 2) : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache if this is the first call */
    if (computed_last == (idx_t)-1) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        global_sum = cache[0];
        computed_last = 0;
    }

    if (n <= computed_last) {
        return cache[(size_t)n];
    }

    /* Compute missing indices up to n */
    ensure_capacity((size_t)n + 1);

    /* Fill indices 1 .. 33 (if not already done) */
    for (idx_t idx = computed_last + 1; idx <= n && idx < 34; ++idx) {
        const seq_t p = (seq_t)2;          /* numerator coefficient for 1 <= n <= 33 */
        seq_t val = p + (seq_t)12 * global_sum;
        cache[(size_t)idx] = val;
        global_sum += val;
    }

    /* If we have just filled up to at least index 33, prepare window_sum */
    if (computed_last < 33 && n >= 33) {
        /* Sum of a(1)..a(33) = global_sum - a(0) */
        window_sum = global_sum - cache[0];
    }

    /* Compute indices 34 .. n */
    for (idx_t idx = (idx_t)34; idx <= n; ++idx) {
        seq_t val;
        if (idx == 34) {
            /* a(34) = 12*window_sum - 78*a(0) + 1 */
            val = (seq_t)12 * window_sum
                - (seq_t)78 * cache[0]
                + (seq_t)1;
        } else {
            /* a(idx) = 12*window_sum - 78*a(idx-34) */
            val = (seq_t)12 * window_sum
                - (seq_t)78 * cache[(size_t)(idx - 34)];
        }
        cache[(size_t)idx] = val;

        /* Update window_sum for next step: add val, subtract a(idx-33) */
        window_sum += val;
        if (idx >= 33) {
            window_sum -= cache[(size_t)(idx - 33)];
        }
    }

    computed_last = n;
    return cache[(size_t)n];
}
