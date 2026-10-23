/*
 * A115081  Column 0 of triangle A115080.
 * https://oeis.org/A115081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} Catalan(n-k) * Catalan(k) * (k+1)! *
 *            C(n,2k) / 2^k (OEIS A115081)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cat = NULL;     /* Catalan numbers */
static idx_t cat_cap = 0;     /* size of cat array (largest index+1) */

static seq_t *fact = NULL;    /* factorial values */
static idx_t fact_cap = 0;    /* size of fact array (largest index+1) */

/* Ensure Catalan numbers up to index n are computed. */
static void ensure_cat(idx_t n)
{
    if ((idx_t)cat_cap > n) return;           /* already enough */

    size_t new_cap = (size_t)(n + 1);
    cat = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
    if (cat_cap == 0) {
        cat[0] = (seq_t)1;
        cat_cap = 1;
    }
    for (idx_t i = cat_cap; i <= n; ++i) {
        /* Catalan recurrence: C_i = C_{i-1} * (4i-2) / (i+1) */
        seq_t numerator   = cat[i - 1] * ( (seq_t)(4 * (i - 1) + 2) );
        cat[i] = numerator / ( (seq_t)(i + 1) );
    }
    cat_cap = new_cap;
}

/* Ensure factorials up to index n+1 are computed. */
static void ensure_fact(idx_t n)
{
    if ((idx_t)fact_cap > n + 1) return;       /* already enough */

    size_t new_cap = (size_t)(n + 2);          /* need indices 0..n+1 */
    fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (fact_cap == 0) {
        fact[0] = (seq_t)1;
        fact_cap = 1;
    }
    for (idx_t i = fact_cap; i <= n + 1; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Precompute needed Catalan numbers and factorials. */
    ensure_cat(n);
    ensure_fact(n);   /* factorials up to (n+1)! are required */

    idx_t max_k = n / 2;
    seq_t sum = 0;
    seq_t binom = (seq_t)1;   /* C(n,0) */
    seq_t pow2k = (seq_t)1;   /* 2^0 */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = cat[n - k];
        term *= cat[k];
        term *= fact[k + 1];
        term *= binom;
        term /= pow2k;        /* exact division */
        sum += term;

        if (k < max_k) {
            /* Next binomial C(n,2(k+1)) from C(n,2k) */
            seq_t t1   = (seq_t)(n - 2 * k);
            seq_t t2   = (seq_t)(n - 2 * k - 1);
            seq_t denom = ((seq_t)(2 * k + 1)) * ((seq_t)(2 * k + 2));
            binom = binom * t1 * t2 / denom;

            pow2k *= (seq_t)2;   /* next power of two */
        }
    }
    return sum;
}
