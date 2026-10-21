/*
 * A235038  E.g.f. satisfies: A(x) = sinh(x + Integral A(x) dx).
 * https://oeis.org/A235038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A235038 – E.g.f. satisfies A'(x) = (1 + A(x))*sqrt(1 + A(x)^2).
   Let a(n) be the nth term (offset 1).  The following recurrence, derived
   from the differential equation, uses only integer arithmetic:

   b(0) = 1
   a(1) = 1,     a(0) = 0
   For m ≥ 1
       b(m) = ( Σ_{k=0}^{m} C(m,k) a(k) a(m-k)
                – Σ_{k=1}^{m-1} C(m,k) b(k) b(m-k) ) / 2

   For n ≥ 2
       a(n) = b(n-1) + Σ_{k=0}^{n-1} C(n-1,k) a(k) b(n-1-k)

   The function memoises already computed terms so calls may be made in any
   order.  All arithmetic is performed in the type seq_t; division by 2 is
   exact by construction.
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_arr = NULL;   /* a[0..]  (a[0] = 0, a[1] = 1) */
static seq_t *b_arr = NULL;   /* b[0..]  (b[0] = 1)         */
static idx_t  a_max = 0;      /* highest index for which a_arr[i] is defined */
static size_t alloc_cap = 0;  /* current allocation capacity */

/* Ensure the arrays are allocated to store indices 0..n and initialise
   newly allocated slots (not strictly necessary but keeps values clear). */
static void ensure_capacity(idx_t n)
{
    if (alloc_cap >= (size_t)n + 1) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    a_arr = new_a;
    b_arr = new_b;

    for (size_t i = alloc_cap; i < new_cap; ++i) {
        a_arr[i] = (seq_t)0;
        b_arr[i] = (seq_t)0;
    }
    alloc_cap = new_cap;

    /* initialise base cases if they haven't been set yet */
    if (a_max == 0) {
        a_arr[0] = (seq_t)0;   /* not part of the sequence but needed in the recurrence */
        a_arr[1] = (seq_t)1;   /* a(1) = 1  (offset 1) */
        b_arr[0] = (seq_t)1;   /* b(0) = 1   from sqrt(1 + A^2) */
        a_max   = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety; sequence is 1‑based */

    /* allocate enough space */
    ensure_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t m = a_max; m < n; ++m) {
        /* ---- compute b(m) for m ≥ 1 ---- */
        seq_t sum_a = (seq_t)0;
        seq_t sum_b = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0) {
                binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            }

            seq_t ak   = a_arr[k];
            seq_t amk  = a_arr[m - k];
            sum_a += binom * ak * amk;

            if (k > 0 && k < m) {
                seq_t bk   = b_arr[k];
                seq_t bmkm = b_arr[m - k];
                sum_b += binom * bk * bmkm;
            }
        }

        b_arr[m] = (sum_a - sum_b) / (seq_t)2;

        /* ---- compute a(m+1) ---- */
        seq_t sum_mix = (seq_t)0;
        binom = (seq_t)1;           /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0) {
                binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            }
            seq_t ak   = a_arr[k];
            seq_t bmkm = b_arr[m - k];
            sum_mix += binom * ak * bmkm;
        }

        a_arr[m + 1] = b_arr[m] + sum_mix;
    }

    a_max = n;   /* mark that all indices up to n are computed */
    return a_arr[n];
}
