/*
 * A124237  Riordan array (1/(1-2x), x/((1-x)(1-2x))).
 * https://oeis.org/A124237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124237 (Riordan array)
    a(n) is obtained by flattening the triangle T(n,k) row‑by‑row.
    For a given index n:
        let r = floor((√(8n+1)-1)/2)   // row number
        let k = n - r(r+1)/2          // column number in that row
        a(n) = T(r,k) where
        T(r,k) = Σ_{j=0}^{r-k} C(r-j,k) * C(r+k,j)   (C is binomial coefficient)
*/
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
       Since n <= 10000 roughly, a simple linear scan is fast and safe. */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 <= n) {
        ++r;
    }
    r--;                                 // now r satisfies r(r+1)/2 <= n

    /* Compute the starting index of row r (triangular number). */
    idx_t row_start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - row_start;             /* column in row r */

    /* Compute T(r,k) via the explicit sum.  All arithmetic is in seq_t. */
    seq_t sum = binom_n_k(r, k);          /* j = 0 term */
    idx_t maxj = r - k;                   /* upper limit of j */

    for (idx_t j = 1; j <= maxj; ++j) {
        seq_t c1 = binom_n_k(r - j, k);
        seq_t c2 = binom_n_k(r + k, j);
        sum += c1 * c2;
    }
    return sum;
}
