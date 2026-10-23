/*
 * A161971  E.g.f. satisfies: A(x) = exp( x*exp( x*A'(x) ) ), where A'(x) =
 *          d/dx A(x).
 * https://oeis.org/A161971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies A(x) = exp( x * exp( x*A'(x) ) ) Computed via a
 *            recurrence for the EGF coefficients. Reference implementation
 *            (C11/C++17)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The types seq_t and idx_t are supplied by the build environment. */

 /* Cached arrays holding computed values. */
static seq_t *a_vals = NULL;
static seq_t *t_vals = NULL;
static size_t  cap   = 0;        /* current capacity of the arrays */
static int     init  = 0;        /* flag: has the first element been set? */

/* Ensure the arrays can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;          /* number of elements required */
    if (need <= cap)
        return;                           /* already enough */

    size_t new_cap = cap;
    if (new_cap == 0)
        new_cap = 64;                     /* initial capacity */
    while (new_cap < need)
        new_cap <<= 1;                    /* grow by powers of two */

    seq_t *new_a = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    seq_t *new_t = (seq_t *)realloc(t_vals, new_cap * sizeof(seq_t));
    if (!new_a || !new_t) {
        /* Allocation failed; abort. */
        exit(EXIT_FAILURE);
    }
    a_vals = new_a;
    t_vals = new_t;

    /* Initialize the newly allocated slots to zero. */
    for (size_t i = cap; i < new_cap; ++i) {
        a_vals[i] = 0;
        t_vals[i] = 0;
    }
    cap = new_cap;
}

/* Return the n-th term of A161971. */
seq_t
a(idx_t n)
{
    ensure_capacity(n);

    /* First call: set the base values a(0)=t(0)=1. */
    if (!init) {
        a_vals[0] = 1;
        t_vals[0] = 1;
        init = 1;
    }

    /* Compute coefficients up to index n (increasing order). */
    for (idx_t i = 1; i <= n; ++i) {
        /* ----- compute a[i] ----------------------------------- */
        seq_t sum_a = 0;
        seq_t binom = 1;      /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom;
            term *= (seq_t)(k + 1);
            term *= t_vals[k];
            term *= a_vals[i - 1 - k];
            sum_a += term;

            if (k + 1 < i)
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
        }
        a_vals[i] = sum_a;

        /* ----- compute t[i] ----------------------------------- */
        seq_t sum_t = 0;
        binom = 1;            /* C(i-1,0) */

        for (idx_t j = 0; j < i; ++j) {
            seq_t term = binom;
            term *= (seq_t)(j + 1);
            term *= a_vals[j + 1];
            term *= t_vals[i - 1 - j];
            sum_t += term;

            if (j + 1 < i)
                binom = binom * (seq_t)(i - 1 - j) / (seq_t)(j + 1);
        }
        t_vals[i] = sum_t;
    }

    return a_vals[n];
}
