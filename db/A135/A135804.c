/*
 * A135804  Seventh column (k=6) of triangle A134832 (circular succession
 *          numbers).
 * https://oeis.org/A135804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) * sum_{s=0}^n (-1)^s * binom(n,s) *
 *            (n-s-1)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              with the convention (−1)! = 1 (used when s = n).                 */

#include <stddef.h>
#include "../utils/binom.h"

/* ----- factorial cache --------------------------------------------------- */
static seq_t *fact_cache = NULL;
static size_t fact_cap   = 0;

static void ensure_factorials(idx_t n)
{
    /* No work if already initialized up to n. */
    if (fact_cache && fact_cap > n) return;

    /* Allocate or grow the factorial table to store 0! … n!. */
    size_t new_cap = n + 1;
    seq_t *new_fact = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!new_fact) return;                 /* allocation failed – ignore */

    if (!fact_cache) {                     /* first time: build from 0 upward */
        new_fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
        fact_cache = new_fact;
        fact_cap   = new_cap;
    } else {                               /* extend the existing table */
        for (idx_t i = fact_cap; i <= n; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
        fact_cache = new_fact;
        fact_cap   = new_cap;
    }
}

/* ----- sequence ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* binomial(n+6,6) */
    seq_t binom_total = binom(n + 6, 6);

    /* factorials needed for the summation */
    ensure_factorials(n);

    /* compute the alternating sum */
    seq_t sum = (seq_t)0;
    for (idx_t s = 0; s <= n; ++s) {
        seq_t term;
        if (s == n) {
            /* (n−s−1)! = (−1)! is defined as 1, and binom(n,n)=1 */
            term = (seq_t)1;
        } else {
            /* term = fact[n] / (fact[s] * (n−s))  == binom(n,s) * fact[n−s−1] */
            term = fact_cache[n];
            term /= fact_cache[s];
            term /= (seq_t)(n - s);
        }

        if (s & 1)
            sum -= term;
        else
            sum += term;
    }

    return binom_total * sum;
}
