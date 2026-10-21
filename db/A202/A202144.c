/*
 * A202144  L.g.f.: (-1/3)*log( Sum_{n>=0} (2*n+1)*(-x)^(n*(n+1)/2) ).
 * https://oeis.org/A202144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202144
    The sequence is computed by the following recurrence.
    Let T_k = k(k+1)/2  (triangular numbers) and
        S_k = (2k+1)·(-1)^{T_k}   (non‑zero only for k≥1).

    a(1) = 1
    For n ≥ 2:
        term1   = -(n/3) · S_n  (0 if n is not triangular)
        sum_pos = Σ_{k: T_k < n, S_k>0}   S_k   · a(n-T_k)
        sum_neg = Σ_{k: T_k < n, S_k<0}  |S_k| · a(n-T_k)

        a(n) = sum_neg + term1 - sum_pos
    All arithmetic is integer only; only the helper mul_small() is used for
    multiplication by a small integer.  The algorithm runs in
    O(n √n) time and O(n) memory, which is well under the limits for
    n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *a_arr      = NULL;      /* memoised values a(1)…a(n)      */
static idx_t  max_n_computed = 0;     /* largest n for which a_arr has been filled */

/* triangular indices and coefficients */
static idx_t  *tri_arr    = NULL;     /* T_k = k(k+1)/2,  k≥0          */
static int64_t *coeff_arr = NULL;     /* S_k = (2k+1)*(−1)^{T_k}       */
static idx_t  tri_max = 0;            /* largest k currently available */

/* extend triangular data up to new_kmax (k≥1) */
static void extend_tri_arrays(idx_t new_kmax)
{
    if (tri_arr == NULL) {
        tri_arr    = (idx_t *)malloc((new_kmax + 1) * sizeof(idx_t));
        coeff_arr = (int64_t *)malloc((new_kmax + 1) * sizeof(int64_t));
        if (!tri_arr || !coeff_arr) return;   /* allocation failed */
        for (idx_t k = 1; k <= new_kmax; ++k) {
            tri_arr[k] = k * (k + 1) / 2;
            int64_t c  = (int64_t)(2 * k + 1);
            if (tri_arr[k] & 1ULL) c = -c;
            coeff_arr[k] = c;
        }
        tri_max = new_kmax;
    } else if (tri_max < new_kmax) {
        tri_arr    = (idx_t *)realloc(tri_arr,    (new_kmax + 1) * sizeof(idx_t));
        coeff_arr = (int64_t *)realloc(coeff_arr, (new_kmax + 1) * sizeof(int64_t));
        if (!tri_arr || !coeff_arr) return;
        for (idx_t k = tri_max + 1; k <= new_kmax; ++k) {
            tri_arr[k] = k * (k + 1) / 2;
            int64_t c  = (int64_t)(2 * k + 1);
            if (tri_arr[k] & 1ULL) c = -c;
            coeff_arr[k] = c;
        }
        tri_max = new_kmax;
    }
}

/* ensure that a_arr contains values up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n_computed) return;

    /* determine required number of triangular indices */
    idx_t k = 1;
    while (k * (k + 1) / 2 <= n) ++k;
    idx_t new_kmax = k - 1;
    extend_tri_arrays(new_kmax);

    /* resize a_arr */
    a_arr = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
    if (!a_arr) return;  /* allocation failed; caller will get 0 */
    if (max_n_computed == 0) a_arr[0] = 0;   /* dummy initial value */

    /* compute a values from max_n_computed+1 … n */
    for (idx_t nidx = max_n_computed + 1; nidx <= n; ++nidx) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        __int128 term1 = 0;

        /* loop over all triangular indices ≤ nidx */
        for (idx_t kk = 1; kk <= tri_max && tri_arr[kk] <= nidx; ++kk) {
            idx_t tri = tri_arr[kk];
            int64_t S  = coeff_arr[kk];

            if (tri == nidx) {
                /* term1 = -(n/3)*S_n  (exact division by 3) */
                term1 = - ( (__int128)nidx * S ) / 3;
            } else {
                seq_t prev = a_arr[nidx - tri];
                if (S > 0)
                    sum_pos = sum_pos + mul_small(prev, (unsigned)(S));
                else
                    sum_neg = sum_neg + mul_small(prev, (unsigned)(-S));
            }
        }

        __int128 result = (__int128)sum_neg - (__int128)sum_pos + term1;
        a_arr[nidx] = (seq_t)result;
    }

    max_n_computed = n;
}

/* ------------------------------------------------------------ */
/*  Public function: sequence A202144, 1‑based indexing         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_capacity(n);
    return a_arr[n];
}
