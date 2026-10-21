/*
 * A088719  Numbers that can be represented as a^7 + b^7, with 0 < a < b, in
 *          exactly one way.
 * https://oeis.org/A088719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A088719: numbers that can be written as a^7 + b^7 with 0 < a < b in exactly one way.
   Implementation: generate the sums of all pairs (a,b) with 1≤a<b≤limit, paste duplicates,
   sort them and keep unique values.  `ensure_terms` grows the search limit until the
   required index n is reached.  The algorithm is O(n·L) with L ≈ sqrt(2n),
   which is fast enough for n ≤ 10 000. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* pow7(x) computes x^7 using only integer arithmetic */
static inline seq_t pow7(idx_t x)
{
    seq_t r = (seq_t)x;
    r *= (seq_t)x; /* x^2 */
    r *= (seq_t)x; /* x^3 */
    r *= (seq_t)x; /* x^4 */
    r *= (seq_t)x; /* x^5 */
    r *= (seq_t)x; /* x^6 */
    r *= (seq_t)x; /* x^7 */
    return r;
}

/* Creates a sorted list of the first "need" unique sums a^7+b^7 with 1≤a<b≤limit.
   The result is stored in `out` (of size at least `need`) and the number of
   produced terms is returned.  `out` must be freed by the caller. */
static idx_t build_unique_terms(idx_t limit, seq_t *out, idx_t need)
{
    if (limit < 2) return 0;

    /* Arrays for the current b index and the sum for each a. */
    idx_t *cur_b = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    seq_t *a_pow = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    seq_t *sum   = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!cur_b || !a_pow || !sum) {
        free(cur_b); free(a_pow); free(sum);
        return 0;
    }

    /* Pre‑compute a^7 for all a and initialise b = a+1. */
    for (idx_t a = 1; a <= limit; ++a) {
        a_pow[a] = pow7(a);
        cur_b[a] = a + 1;
        sum[a]   = a_pow[a] + pow7(cur_b[a]);
    }

    seq_t previous = (seq_t)0;   /* 0 cannot be a valid sum (min is 129) */
    idx_t count    = 0;

    while (count < need) {
        /* find the smallest current sum */
        idx_t  best = 1;
        seq_t minval = sum[1];
        for (idx_t a = 2; a <= limit; ++a) {
            if (sum[a] < minval) {
                minval = sum[a];
                best   = a;
            }
        }

        /* Add unique values only */
        if (count == 0 || minval != previous) {
            out[count++] = minval;
            previous = minval;
        }

        /* Advance b for the chosen a */
        ++cur_b[best];
        sum[best] = a_pow[best] + pow7(cur_b[best]);
    }

    free(cur_b); free(a_pow); free(sum);
    return count;
}

/* Global cache of the sequence */
static seq_t *global_terms  = NULL;   /* sorted ascending unique sums */
static idx_t  global_count  = 0;      /* number of terms stored */
static idx_t  global_limit  = 0;      /* limit of a used during last build */

/* Ensure that at least `n` terms of the sequence have been generated. */
static void ensure_terms(idx_t n)
{
    if (n <= global_count) return;            /* already available */

    /* Estimate an initial limit based on pair count. */
    idx_t estimate = 2;
    if (global_limit < estimate) estimate = global_limit + 1;
    while (estimate * (estimate - 1) / 2 < n) estimate++;

    idx_t limit = estimate;
    while (1) {
        /* Allocate temporary output buffer.  The final count cannot exceed n,
           but we allocate a little extra to avoid re‑allocations. */
        seq_t *tmp = (seq_t *)malloc((n + 10) * sizeof(seq_t));
        if (!tmp) return;                      /* out of memory – give up */

        idx_t tmp_count = build_unique_terms(limit, tmp, n);
        if (tmp_count >= n) {
            /* Enough terms produced.  Replace global cache. */
            free(global_terms);
            global_terms = tmp;
            global_count = tmp_count;
            global_limit = limit;
            return;
        }
        free(tmp);
        /* Not enough – increase the search limit */
        limit <<= 1;
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour for n<1 undefined */

    ensure_terms(n);
    if (n <= global_count) return global_terms[n - 1];
    return (seq_t)0;                     /* should not happen */
}
