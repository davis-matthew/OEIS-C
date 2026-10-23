/*
 * A026865  Number of labeled chain graphs.
 * https://oeis.org/A026865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Σ_{k=1..n} binomial(n,k)*b(k)*a(n-k)*2^{k*(n-k)} , with
 *            a(0)=b(0)=1 and b(n) = -Σ_{k=0..n-1}
 *            binomial(n,k)*b(k)*2^{(n-k)*(n-k-1)/2}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *a_vals = NULL;      /* a[0..] */
static seq_t *b_vals = NULL;      /* b[0..] */
static size_t a_cap = 0;          /* allocated size of a_vals */
static size_t b_cap = 0;          /* allocated size of b_vals */
static idx_t last_n = 0;          /* largest n that has been computed */

/* Ensure that an array has at least 'need' elements */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need)
    {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure – let abort */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Main entry function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 as helper for recurrence */

    /* Allocate space for indices 0..n */
    ensure_capacity(&a_cap, &a_vals, (size_t)n + 1);
    ensure_capacity(&b_cap, &b_vals, (size_t)n + 1);

    /* Initialise base values if not yet done */
    if (last_n == 0)
    {
        a_vals[0] = (seq_t)1;
        b_vals[0] = (seq_t)1;
    }

    /* Compute all values up to n that have not been computed yet */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        /* ----- compute b[i] ----- */
        seq_t sum_b = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
        {
            seq_t bin = binomial(i, k);                /* C(i,k) */
            idx_t nk   = i - k;
            idx_t exp  = nk * (nk - 1) / 2;            /* C(nk,2) */
            seq_t pw   = pow2(exp);                    /* 2^exp */
            seq_t term = bin * b_vals[k] * pw;
            sum_b -= term;                              /* leading '-' */
        }
        b_vals[i] = sum_b;

        /* ----- compute a[i] ----- */
        seq_t sum_a = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
        {
            seq_t bin = binomial(i, k);                /* C(i,k) */
            idx_t exp  = k * (i - k);                  /* k*(i-k) */
            seq_t pw   = pow2(exp);                    /* 2^exp */
            seq_t term = bin * b_vals[k] * a_vals[i - k] * pw;
            sum_a -= term;                              /* leading '-' */
        }
        a_vals[i] = sum_a;
    }

    last_n = n;
    return a_vals[n];
}
