/*
 * A152456  a(n)=1*(n+2)!-2*(n+1)!-3*n!.
 * https://oeis.org/A152456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+2)! - 2*(n+1)! - 3*n! == n! * (n² + n - 3)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached factorials: fact_cache[k] = k!  (fact_cache[0] = 1) */
static seq_t *fact_cache = NULL;
static idx_t fact_capacity = 0;   /* allocated size of fact_cache */
static idx_t fact_size = 0;       /* number of factorials that have been
                                   * computed (fact_size ≤ fact_capacity) */

/* Ensure factorial cache contains at least factorials up to idx */
static void ensure_fact(idx_t idx)
{
    if (idx < fact_size) return;            /* already present */

    /* Grow capacity if necessary */
    idx_t new_cap = fact_capacity ? fact_capacity : 8;
    while (new_cap <= idx) new_cap <<= 1;   /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    fact_cache   = tmp;
    fact_capacity = new_cap;

    /* Compute missing factorials */
    for (idx_t i = fact_size; i <= idx; ++i) {
        if (i == 0)
            fact_cache[0] = (seq_t)1;
        else
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_size = idx + 1;
}

seq_t a(idx_t n)
{
    ensure_fact(n);
    seq_t fac  = fact_cache[n];
    seq_t n_sq = (seq_t)n * (seq_t)n;               /* n² */
    seq_t term = n_sq + (seq_t)n - (seq_t)3;        /* n² + n - 3 */
    return fac * term;
}
