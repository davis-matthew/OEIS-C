/*
 * A138779  Triangle read by rows: T(n,k)=k*binomial(n-2k,3k+1) (n>=6,
 *          0<=k<=(n-1)/5).
 * https://oeis.org/A138779
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k * binomial(r-2*k, 3*k+1) where (r,k) are obtained by
 *            flattening the triangle T(r,k)=k*binomial(r-2k,3k+1) row by row
 *            (omit zeros), and offset 6 sets a(6)=T(6,1)=1
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 6)                 /* below the offset – value is undefined */
        return 0;

    /* position of the n-th term within the flattened triangle (1‑based) */
    idx_t pos = n - 5;

    /* Determine block J (each block of 5 rows contains J elements per row) */
    idx_t J = 1;
    while (5 * J * (J + 1) / 2 < pos)   /* cumulative elements up to block J */
        ++J;

    /* Number of elements before block J */
    idx_t prev_total = 5 * (J - 1) * J / 2;

    /* Position within block J (1‑based) */
    idx_t rem = pos - prev_total;

    /* Row offset within the block and column k */
    idx_t row_offset = (rem - 1) / J;   /* 0‑based row index within block */
    idx_t k = (rem - 1) % J + 1;        /* column, 1‑based (k≥1) */

    /* Row index r */
    idx_t r = 6 + (J - 1) * 5 + row_offset;

    /* Compute the triangle entry */
    idx_t a_val = r - 2 * k;            /* upper parameter of binomial */
    idx_t b_val = 3 * k + 1;            /* lower parameter of binomial */
    seq_t term = binomial(a_val, b_val);
    term = term * (seq_t)k;             /* multiply by k */

    return term;
}
