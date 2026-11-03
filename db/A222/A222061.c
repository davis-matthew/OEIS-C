/*
 * A222061  Triangle read by rows: coefficients of second-order
 *          hypergeometric-harmonic polynomials.
 * https://oeis.org/A222061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row(n), k(n)) where * T(n,k) = Stirling2(n,k) * k! *
 *            H2(k), * H2(k) = Σ_{i=1..k} (k-i+1)/i. * The triangle is read
 *            row by row starting with a(0) = T(0,0) = 0. * The sequence uses
 *            0‑indexed offset
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Computes K(k) = k! * H2(k) as integer for k = 0..maxk.           */
static seq_t *
K_array(idx_t maxk, seq_t **fac_ptr)
{
    seq_t *fac = *fac_ptr;
    seq_t *K = mem_new(seq_t, (maxk + 1));
    K[0] = 0;
    for (idx_t kk = 1; kk <= maxk; kk++) {
        seq_t sum = 0;
        for (idx_t m = 1; m <= kk; m++) {
            /* fac[kk]/m is integer because m divides kk!          */
            seq_t term = fac[kk] / m;
            term *= (kk - m + 1);
            sum += term;
        }
        K[kk] = sum;
    }
    return K;
}

/* Computes factorials up to n, stored in an array, needed for K(k).  */
static seq_t *factorial_array(idx_t n)
{
    seq_t *fac = mem_new(seq_t, (n + 1));
    fac[0] = 1;
    for (idx_t i = 1; i <= n; i++) fac[i] = fac[i - 1] * i;
    return fac;
}

/* Helper that computes the next row index (row count) and the
   position k within the row for a given linear index n.         */
static void
row_and_k(idx_t n, idx_t *row, idx_t *k)
{
    idx_t r = 0;
    idx_t prefix = 0;
    /* Add rows until the next row would exceed n.                  */
    while (prefix + r + 1 <= n) {
        prefix += r + 1;
        r++;
    }
    *row = r;                      /* row index */
    *k   = n - prefix;             /* k within row, 0‑based          */
}

/* Computes Stirling numbers of the second kind up to a given
   row 'max_row' and column 'max_col' using dynamic programming.   */
static seq_t
stirling2(idx_t max_row, idx_t max_col, idx_t target_row, idx_t target_col)
{
    if (target_col > target_row)
        return 0;                 /* other entries are zero          */

    /* Arrays of length max_row+1 are allocated on the heap.         */
    seq_t *prev = mem_new(seq_t, (max_row + 1));
    seq_t *cur  = mem_new(seq_t, (max_row + 1));

    /* Initial values: S(0,0)=1, others zero.                       */
    prev[0] = 1;
    for (idx_t j = 1; j <= max_row; j++) prev[j] = 0;

    /* Build up rows 1..max_row.                                     */
    for (idx_t i = 1; i <= max_row; i++) {
        cur[0] = 0;
        for (idx_t j = 1; j <= i; j++) {
            cur[j] = prev[j] * j + prev[j - 1];
        }
        /* Copy only the computed part of the new row.               */
        for (idx_t j = 0; j <= i; j++) prev[j] = cur[j];
    }

    seq_t result = prev[target_col];
    mem_free(prev);
    mem_free(cur);
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row 'r' and column 'k' corresponding to linear index n */
    idx_t r, k;
    row_and_k(n, &r, &k);

    /* For k == 0, T(r,0) = 0 by definition (H2(0)=0).              */
    if (k == 0) return (seq_t)0;

    /* Precompute factorials up to 'k' as they are needed for K(k).     */
    seq_t *fac = factorial_array(k);
    seq_t *K   = K_array(k, &fac);

    /* Compute Stirling2(r,k).                                         */
    seq_t S = stirling2(r, k, r, k);

    /* Final result: Stirling2 * K(k).                                */
    seq_t result = S * K[k];

    /* Free temporaries.                                              */
    mem_free(fac);
    mem_free(K);

    return result;
}
