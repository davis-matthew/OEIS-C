/*
 * A177840  Consider the n pairs (1,2), ..., (2n-1,2n); a(n) is the number of
 *          permutations of [ 2n ] with no two fixed points for any pair.
 * https://oeis.org/A177840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A177840
   Formula: a(n) = Σ_{j=0}^{n} C(n,j)·(2n–2j)!·(–1)^j
   (indices start at n = 0) */

#include <stdlib.h>

/* Static cache of factorials up to the largest needed value. */
static seq_t *fact_cache = NULL;      /* array of factorials */
static idx_t  max_fact_cached = 0;    /* largest index currently in cache */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Ensure factorial cache contains all factorials up to 2*n. */
    idx_t need = 2 * n;
    if (need > max_fact_cached) {
        size_t new_size = (size_t)(need + 1);
        seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* In unlikely event of allocation failure, return 1. */
            return (seq_t)1;
        }
        fact_cache = new_cache;

        idx_t start = (max_fact_cached == 0) ? 0 : (max_fact_cached + 1);
        if (start == 0) {
            fact_cache[0] = (seq_t)1;   /* 0! = 1 */
            start = 1;
        }
        for (idx_t i = start; i <= need; ++i) {
            /* i! = (i-1)! * i */
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        max_fact_cached = need;
    }

    /* Use a running binomial coefficient: C(n,0)=1. */
    seq_t comb = (seq_t)1;
    seq_t sum = (seq_t)0;

    for (idx_t j = 0; j <= n; ++j) {
        /* Index of the factorial we need: (2n-2j) */
        idx_t fact_idx = need - 2 * j;
        seq_t term = comb * fact_cache[fact_idx];

        /* Add or subtract based on (-1)^j. */
        if ((j & 1) == 0) {
            sum += term;
        } else {
            sum -= term;
        }

        /* Update binomial coefficient for the next j. */
        if (j != n) {
            comb = comb * (seq_t)(n - j) / (seq_t)(j + 1);
        }
    }

    return sum;
}
