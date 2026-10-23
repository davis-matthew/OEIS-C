/*
 * A185958  Accumulation array of the array max{n,k}, by antidiagonals.
 * https://oeis.org/A185958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{i=1}^{i0} sum_{j=1}^{j0} max(i,j) where (i0,j0) are
 *            the * indices on the antidiagonal that contains the n‑th element
 *            when the 2‑D array * B(i,j)=Σ_{r=1}^{i} Σ_{s=1}^{j} max(r,s) is
 *            read anti‑diagonally (i increasing * on each diagonal). The
 *            closed form for B(i,j) with a=min(i,j), b=ma...
 * Range:     u8 n <= 94, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _diagonal_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_diagonal_of.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset is 1, but guard */

    /* step 1: diagonal */
    idx_t d = _diagonal_of(n);

    /* step 2: position on diagonal */
    idx_t prevCnt = ((__uint128_t)(d - 1) * (d - 2)) / 2;
    idx_t t = n - prevCnt;              /* 1‑based position */

    /* step 3: indices */
    idx_t i = t;
    idx_t j = d - t;

    /* step 4: min and max */
    idx_t a_min = i < j ? i : j;
    idx_t a_max = i < j ? j : i;

    /* step 5: compute the closed form */
    seq_t a_min_t = (seq_t)a_min;
    seq_t a_max_t = (seq_t)a_max;
    seq_t term = a_min_t *
                 ( (a_min_t * a_min_t - (seq_t)1)
                   + (seq_t)3 * a_max_t * (a_max_t + (seq_t)1) )
                 / (seq_t)6;
    return term;
}
