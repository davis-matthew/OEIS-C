/*
 * A217392  Alternating sums of the squares of the ordered Bell numbers
 *          (number of preferential arrangements) A000670.
 * https://oeis.org/A217392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^{n-k} * t(k)^2 (ordered Bell numbers t
 *            satisfy t(0)=1, t(n)=Σ_{k=0}^{n-1} binom(n,k)*t(k))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for ordered Bell numbers */
static seq_t *t_cache = NULL;   /* t_cache[i] = t(i)   */
static idx_t   t_len   = 0;     /* number of values currently stored in t_cache */
static size_t  t_cap   = 0;     /* allocated capacity of t_cache */

/* Ensure t_cache contains values up to index n inclusive */
static void
ensure_t_upto(idx_t n)
{
    if (n < t_len)               /* already computed */
        return;

    /* Grow allocation if needed */
    if ((size_t)(n + 1) > t_cap) {
        size_t new_cap = t_cap == 0 ? 16 : t_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;                 /* double until large enough */

        /* realloc returns void*, cast so it compiles in C++ */
        t_cache = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
        t_cap   = new_cap;
    }

    /* Compute missing t values */
    if (t_len == 0) {          /* initialise the first value */
        t_cache[0] = (seq_t)1; /* t(0)=1 */
        t_len = 1;
    }

    /* Recurrence for t(m) = Σ_{k=0}^{m-1} binom(m,k) * t(k)  */
    for (idx_t m = t_len; m <= n; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binom(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            sum += binom * t_cache[k];

            /* Prepare next binomial coefficient:
               binom(m,k+1) = binom(m,k) * (m-k) / (k+1)  */
            if (k + 1 < m)
                binom = (binom * (m - k)) / ((k + 1));
        }
        t_cache[m] = sum;
    }

    t_len = n + 1;   /* we now have t(0) .. t(n) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Make sure all required t(k) are known */
    ensure_t_upto(n);

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* t(k)^2 */
        seq_t sq = t_cache[k];
        sq *= sq;

        /* Determine sign: (-1)^{n-k}  */
        if ( ((n ^ k) & 1) == 0 )
            sum_pos += sq;
        else
            sum_neg += sq;
    }

    /* Since the result is non‑negative: sum_pos ≥ sum_neg */
    return sum_pos - sum_neg;
}
