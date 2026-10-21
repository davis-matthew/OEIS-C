/*
 * A214776  Number A(n,k) of standard Young tableaux of shape [n*k,n]; square
 *          array A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A214776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 113, u64 n <= 290,
 *            u128 n <= 766, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A214776: Number A(n,k) of standard Young tableaux of shape [n*k,n]. */
/* Formula used: for n,k ≥ 1, A(n,k) = (k+1)·C((k+1)n-1, n-1) – C((k+1)n, n-1). */
/* Antidiagonal mapping: for a given index n, find d with d(d+1)/2 ≤ n < (d+1)(d+2)/2,
   then i = n - d(d+1)/2, j = d - i.  A(0,*) = 1, A(*,0) = 0 for * > 0. */

#include <stdbool.h>
#include "../utils/binomial.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }

    /* Position on the diagonal: i (row), j (column). */
    idx_t i = n - d * (d + 1) / 2;   /* row index */
    idx_t j = d - i;                /* column index */

    /* Handle border cases. */
    if (i == 0) {           /* first row */
        return 1;
    }
    if (j == 0) {           /* first column, except (0,0) */
        return 0;
    }

    /* Compute the value using the combinatorial formula. */
    idx_t mult = (j + 1) * i;                        /* (k+1)*n */
    seq_t c1 = binomial(mult - 1, i - 1);           /* C((k+1)n-1, n-1) */
    seq_t c2 = binomial(mult,     i - 1);           /* C((k+1)n,   n-1) */
    return (seq_t)(j + 1) * c1 - c2;
}
