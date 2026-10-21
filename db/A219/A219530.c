/*
 * A219530  Number of functions f:{1,2,...,n}->{1,2,...,n} such that each
 *          component of f is a function on an interval of {1,2,...,n}.
 * https://oeis.org/A219530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A219530
   Number of functions f:{1,2,...,n}->{1,2,...,n} such that each
   component of f is a function on an interval of {1,2,...,n}.
   Recurrence:
        a(0) = 1
        s(0) = 0                                       (unused)
        s(n) = n^n – Σ_{k=1}^{n-1} C(n-1,k-1)·s(k)·(n-k)^{\,n-k}
        a(n) = Σ_{k=1}^{n} s(k)·a(n-k)
   This expression follows from: a(x) = 1/(1–S(x)),
   where S(x) has coefficients s(k).  */

/*  The implementation keeps a static cache of computed values so that
    subsequent calls with arbitrary (possibly decreasing) indices
    are served in O(n^2) time over all values.  It uses the generic
    big_pow function from the utilities library for exponentiation.  */

#include <stdlib.h>
#include "../utils/big_pow.h"

/* static cache */
static seq_t *a_arr = NULL;   /* a values   */
static seq_t *s_arr = NULL;   /* s values   */
static seq_t *p_arr = NULL;   /* i^i values*/
static idx_t  cap   = 0;      /* number of allocated slots (max index + 1) */

/* ensure capacity and initialise new entries; also compute missing values */
static void precompute_to(idx_t n)
{
    if (n + 1 <= cap) return;                 /* already up to date */

    idx_t old_cap = cap;
    idx_t new_cap = n + 1;

    /* resize arrays */
    a_arr = (seq_t *)realloc(a_arr, sizeof(seq_t) * new_cap);
    s_arr = (seq_t *)realloc(s_arr, sizeof(seq_t) * new_cap);
    p_arr = (seq_t *)realloc(p_arr, sizeof(seq_t) * new_cap);

    /* zero initialise new slots (they will be overwritten later) */
    for (idx_t i = old_cap; i < new_cap; ++i) {
        a_arr[i] = 0;
        s_arr[i] = 0;
        p_arr[i] = 0;
    }

    /* initialise the very first entry if this is the first call */
    if (old_cap == 0) {
        a_arr[0] = (seq_t)1;        /* a(0) = 1 */
        s_arr[0] = (seq_t)0;        /* unused */
        p_arr[0] = (seq_t)1;        /* 0^0 = 1 (not used) */
    }

    /* compute values for new indices */
    for (idx_t i = (old_cap > 0 ? old_cap : 1); i <= n; ++i) {
        /* 1. compute i^i */
        p_arr[i] = big_pow((seq_t)i, (seq_t)i);

        /* 2. compute s(i) via inclusion–exclusion */
        seq_t sum_terms = 0;
        seq_t binom     = 1;                     /* C(i-1,0) */

        for (idx_t k = 1; k <= i - 1; ++k) {
            /* term = binom * s(k) * (i-k)^{i-k} */
            seq_t term = binom;
            term *= s_arr[k];
            term *= p_arr[i - k];
            sum_terms += term;

            /* update binom to C(i-1,k) */
            binom = binom * (seq_t)(i - k) / (seq_t)k;
        }
        s_arr[i] = p_arr[i] - sum_terms;

        /* 3. compute a(i) from the convolution */
        seq_t val = 0;
        for (idx_t k = 1; k <= i; ++k) {
            val += s_arr[k] * a_arr[i - k];
        }
        a_arr[i] = val;
    }

    cap = new_cap;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    precompute_to(n);
    return a_arr[n];
}
