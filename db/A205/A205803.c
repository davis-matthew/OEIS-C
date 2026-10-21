/*
 * A205803  E.g.f.: -log( Sum_{n>=0} (-x)^(n*(n+1)/2) / (n*(n+1)/2)! ).
 * https://oeis.org/A205803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n/n! in -log( Sum_{m>=0} (-x)^{m(m+1)/2}
 *            / (m(m+1)/2)! )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n^2) recurrence based on the formal
   identity exp(-A(x)) = Sum_{k>=0} (-x)^{k(k+1)/2} / (k(k+1)/2)! .
   The coefficient c_k of the right‑hand side is 0 except when k is a
   triangular number t_m = m(m+1)/2.  In that case
        c_k = (-1)^k    (k = t_m).
   The recurrence for the a_n (the sequence values) is

        a_n = sum_{k=1}^{n-1} [ c_{n-k} = -1 ?  binom(n-1,k-1)*a_k
                                      : c_{n-k} = +1 ? –binom(n-1,k-1)*a_k
                                      : 0 ] - c_n

   which can be computed efficiently by accumulating the two sums
   sum_pos = Σ_{c_{n-k}=+1} binom(n-1,k-1)*a_k
   sum_neg = Σ_{c_{n-k}=-1} binom(n-1,k-1)*a_k
   then a_n = sum_neg – sum_pos – c_n.  All intermediate sums are
   non‑negative, so no signed arithmetic is required.  The array of
   coefficients c_k is pre‑computed once (it is very sparse).  The
   sequence values are cached so that calls in arbitrary order remain
   O(n^2) for the largest requested n. */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static seq_t *A = NULL;          /* cached a values, 1‑based */
static int8_t *C = NULL;         /* cached c values (-1,0,1) */
static idx_t curr_cap = 0;       /* current allocated size (>= n) */
static idx_t computed = 0;       /* highest n computed so far */
static idx_t tri_limit = 0;      /* highest triangular number set in C[] */

/* extend the array to hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= curr_cap) return;

    idx_t new_cap = (curr_cap == 0) ? 16 : curr_cap * 2;
    while (new_cap < n) new_cap <<= 1;

    A = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
    C = (int8_t *)realloc(C, (new_cap + 1) * sizeof(int8_t));
    assert(A && C);

    /* initialise new entries to zero */
    for (idx_t i = curr_cap + 1; i <= new_cap; ++i) {
        A[i] = (seq_t)0;
        C[i] = 0;
    }

    curr_cap = new_cap;
}

/* set C[k] for k up to n, keeping already set values unchanged          */
static void ensure_triangular(idx_t n)
{
    if (n <= tri_limit) return;
    /* compute triangular numbers up to n */
    for (idx_t m = 0; ; ++m) {
        idx_t t = (m * (m + 1)) / 2;
        if (t > n) break;
        if (t > tri_limit) {
            C[t] = ((t & 1) ? -1 : 1);      /* (-1)^t */
        }
    }
    /* C[0] is always 1 */
    C[0] = 1;
    tri_limit = n;
}

seq_t a(idx_t n)
{
    /* compute up to requested n */
    if (n == 0) return (seq_t)0;          /* not used, but harmless */

    ensure_capacity(n);
    ensure_triangular(n);

    for (idx_t k = computed + 1; k <= n; ++k) {
        /* compute sum_neg and sum_pos for this k */
        seq_t sum_neg = (seq_t)0;
        seq_t sum_pos = (seq_t)0;
        seq_t bin = (seq_t)1;             /* binom(k-1,0) */

        for (idx_t j = 1; j <= k - 1; ++j) {
            seq_t term = bin * A[j];      /* bin * a_j */
            int8_t c_val = C[k - j];

            if (c_val == 1) {
                sum_pos += term;
            } else if (c_val == -1) {
                sum_neg += term;
            }

            /* update bin to binom(k-1, j) */
            bin = bin * (seq_t)(k - j);
            bin = bin / (seq_t)j;
        }

        /* e = sum_neg - sum_pos  (assumed >= 0) */
        seq_t e = sum_neg - sum_pos;

        int8_t c_k = C[k];
        seq_t a_val;
        if (c_k == 1) {           /* c_k = +1 -> subtract 1 */
            a_val = e - (seq_t)1;
        } else if (c_k == -1) {   /* c_k = -1 -> add 1 */
            a_val = e + (seq_t)1;
        } else {                  /* c_k = 0 */
            a_val = e;
        }

        A[k] = a_val;
    }

    computed = n;
    return A[n];
}
