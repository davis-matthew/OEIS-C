/*
 * A122075  Coefficients of a generalized Pell-Lucas polynomial read by rows.
 * https://oeis.org/A122075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the entry T(r,c) of the triangle defined by T(n,k) =
 *            Σ_{j=0..n-k+1} binomial(n-k-j+1, j) * binomial(n-j, k), where r
 *            and c are obtained from the linear index n by r = floor(
 *            (sqrt(8n+1)-1)/2 ), c = n - r(r+1)/2
 * Range:     u8 n <= 37, u16 n <= 122, u32 n <= 412, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is read row‑by‑row, so a(0)=T(0,0), a(1)=T(1,0),
   a(2)=T(1,1), a(3)=T(2,0), … */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1),
       where offset_row(r) = r(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;   /* column in the row */

    /* Compute T(r,c) by the defining sum. */
    seq_t sum = 0;
    idx_t maxj = r - c + 1;  /* upper bound for j */
    for (idx_t j = 0; j <= maxj; ++j) {
        /* Ensure the second binomial has non‑negative parameters. */
        if (r < j + c) {
            break;
        }
        seq_t term1 = binomial(r - c - j + 1, j);
        seq_t term2 = binomial(r - j, c);
        sum += term1 * term2;
    }
    return sum;
}
