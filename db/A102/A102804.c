/*
 * A102804  Let f(n) be the minimal number of distinct nonzero tetrahedral
 *          numbers that add to n (or -1 if n is not a sum of distinct
 *          tetrahedral numbers); sequence gives numbers n for which f(n) = 5.
 * https://oeis.org/A102804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of integers n such that the minimal number of
   distinct non‑zero tetrahedral numbers summing to n is exactly 5. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Names used by the build system: seq_t and idx_t. */

/* Global state for memoisation. */
static seq_t *terms        = NULL;   /* sorted list of n with f(n)=5          */
static idx_t  terms_count  = 0;      /* number of stored terms               */
static idx_t  terms_capacity = 0;    /* allocated capacity of terms array    */

static uint8_t *best        = NULL;   /* minimal number of terms for each sum */
static idx_t    best_size   = 0;      /* best array length (= limit+1)         */

static idx_t    computed_limit = 0;   /* largest limit processed so far       */

/* Recompute all data up to a new upper bound (inclusive). */
static void recompute(idx_t new_limit)
{
    /* Generate all tetrahedral numbers up to new_limit. */
    idx_t t_vals[2048];    /* should hold enough for limits up to ~10⁹ */
    idx_t m = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t t = ((seq_t)k * (k + 1) * (k + 2)) / 6;  /* T_k */
        if ((idx_t)t > new_limit) break;
        t_vals[m++] = (idx_t)t;
    }

    /* Allocate and initialise DP array. */
    size_t dp_len = (size_t)(new_limit + 1);
    uint8_t *new_best = (uint8_t *)malloc(dp_len * sizeof(uint8_t));
    if (!new_best) abort();
    memset(new_best, 255, dp_len * sizeof(uint8_t));  /* 255 = INF */
    new_best[0] = 0;   /* base case: sum 0 needs 0 items */

    /* 0/1 knapsack DP: each tetrahedral number used at most once. */
    for (idx_t i = 0; i < m; ++i) {
        idx_t t = t_vals[i];
        for (idx_t s = new_limit; s >= t; --s) {
            if (new_best[s - t] != 255) {
                uint8_t cand = new_best[s - t] + 1;
                if (cand < new_best[s]) new_best[s] = cand;
            }
        }
    }

    /* Collect sums with minimal representation size exactly 5. */
    idx_t new_count = 0;
    for (idx_t s = 1; s <= new_limit; ++s) {
        if (new_best[s] == 5) {
            if (new_count + 1 > terms_capacity) {
                /* Grow terms buffer. */
                idx_t new_cap = terms_capacity ? terms_capacity * 2 : 64;
                while (new_cap <= new_count) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_capacity = new_cap;
            }
            terms[new_count++] = (seq_t)s;
        }
    }

    /* Replace old DP array with the new one. */
    free(best);
    best = new_best;
    best_size = new_limit + 1;
    computed_limit = new_limit;
    terms_count = new_count;
}

/* Public function required by the OEIS test harness. */
seq_t a(idx_t n)
{
    /* Ensure we have enough terms precomputed for the requested index. */
    if (terms_count < n) {
        /* Start with an initial limit and grow until we have enough terms. */
        idx_t limit = computed_limit ? computed_limit * 2 : 1000;
        while (terms_count < n) {
            recompute(limit);
            limit <<= 1;
        }
    }
    return terms[n - 1];
}
