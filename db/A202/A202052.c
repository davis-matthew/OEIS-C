/*
 * A202052  T(n,k)=Number of (n+2)X(k+2) binary arrays avoiding patterns 001
 *          and 110 in rows and columns.
 * https://oeis.org/A202052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n
 *            and T(n,k)=2*(n+2)*(2*binomial(n+k+3,n+2)-k-2)
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 311, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find antidiagonal coordinates (i,j) for the index n (offset 1). */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n) {
        d++;
    }
    idx_t prev = (d - 2) * (d - 1) / 2;
    idx_t offset = n - prev - 1;          /* 0‑based offset on this diagonal */
    idx_t i = offset + 1;                 /* row index */
    idx_t j = d - i;                      /* column index */

    /* Compute T(i,j) using the empirical formula. */
    seq_t bin  = binomial((idx_t)(i + j + 3), (idx_t)(i + 2));
    seq_t term = bin * 2;
    term -= (seq_t)j;
    term -= 2;
    seq_t factor = ((seq_t)(i + 2)) * 2;
    return factor * term;
}
