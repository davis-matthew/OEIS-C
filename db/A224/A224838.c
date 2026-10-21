/*
 * A224838  Triangle read by rows, obtained from triangle A011973 by reading
 *          that array from right to left along the irregular paths shown in
 *          the figure.
 * https://oeis.org/A224838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained from the reverse binomial R(r,k) where R(r,k) =
 *            binomial(r - floor((k+1)/2), r - floor((3k-1)/2)) for k = 1 ..
 *            floor((2r+2)/3). To read the triangle in the usual left‑to‑right
 *            order we use k = length(r) - pos + 1. The sequence starts at n =
 *            1
 * Range:     u8 n <= 43, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t row_length(idx_t r)
{
    return (2 * r + 2) / 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset, guard anyway */

    idx_t row = 1;
    idx_t sum = 0;
    idx_t len = row_length(row);

    /* Find the row containing position n */
    while (sum + len < n) {
        sum += len;
        ++row;
        len = row_length(row);
    }

    /* Position within the row (1‑based) */
    idx_t pos = n - sum;

    /* Index k in the reverse list R(row,k) */
    idx_t k   = len - pos + 1;

    /* Compute the binomial argument indices */
    idx_t anum = row - (k + 1) / 2;           /* row - floor((k+1)/2) */
    idx_t bden = row - (3 * k - 1) / 2;       /* row - floor((3k-1)/2) */

    return binomial(anum, bden);
}
