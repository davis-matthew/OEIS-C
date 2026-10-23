/*
 * A130749  Triangle A007318*A090181 (as infinite lower triangular matrices) .
 * https://oeis.org/A130749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c), where r = floor((sqrt(8*n+1)-1)/2) (the row
 *            containing index n) c = n – r(r+1)/2 (column within that row)
 *            T(n,k) = 1 if k = 0 ∑_{i=k}^{n} binom(n,i) * N(i,k) otherwise
 *            N(i,k) = binom(i,k) * binom(i,k-1) / i (Narayana numbers)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom_direct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: indices start at 0, and the triangle is flattened row‑by‑row. */

#include <stdint.h>
#include "../utils/binom_direct.h"

/* Return the a(n) value for the A130749 sequence. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = (idx_t)0;
    idx_t hi = n + (idx_t)1;       /* exclusive upper bound */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t tri = mid * (mid + (idx_t)1) / (idx_t)2;
        if (tri <= n)
            lo = mid + (idx_t)1;
        else
            hi = mid;
    }
    idx_t r = lo - (idx_t)1;
    idx_t offset = r * (r + (idx_t)1) / (idx_t)2;
    idx_t c = n - offset;

    if (c == (idx_t)0)
        return (seq_t)1;                       /* T(r,0) = 1 */

    /* Compute T(r,c) = sum_{i=c}^{r} binom(r,i) * N(i,c). */
    seq_t sum = (seq_t)0;
    for (idx_t i = c; i <= r; ++i) {
        seq_t bin_nr_i = binom_direct(r, i);
        seq_t bin_ic   = binom_direct(i, c);
        seq_t bin_ic1  = binom_direct(i, c - (idx_t)1);
        seq_t prod     = bin_ic * bin_ic1;
        seq_t N_ic     = prod / (seq_t)i;      /* exact division */
        sum += bin_nr_i * N_ic;
    }
    return sum;
}
