/*
 * A123160  Triangle read by rows: T(n,k) = n!*(n+k-1)!/((n-k)!*(n-1)!*(k!)^2)
 *          for 0 <= k <= n, with T(0,0) = 1.
 * https://oeis.org/A123160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 129, u64 n <= 423,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A123160: T(n,k) = binomial(n,k) * binomial(n+k-1,k)
   with a(n) obtained by reading the triangle row‑by‑row.
   Row index r and column index k for a given n are
     r = floor((sqrt(1+8*n)-1)/2)
     k = n - r*(r+1)/2
*/

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the first element directly (row 0) */
    if (n == 0) return (seq_t)1;

    /* Find the row index r of the element */
    seq_t inside = (seq_t)8 * (seq_t)n + (seq_t)1;   /* 1 + 8*n */
    seq_t sq = floor_sqrt(inside);                  /* floor(sqrt(1+8*n)) */
    idx_t r = (idx_t)((sq - (seq_t)1) / (seq_t)2);   /* floor((sqrt(1+8n)-1)/2) */

    /* Index of element within its row */
    idx_t sum_before = r * (r + 1) / 2;              /* elements before row r */
    idx_t k = n - sum_before;

    /* Compute the binomial coefficients */
    seq_t bin1 = binomial((idx_t)r, (idx_t)k);        /* C(r, k) */
    idx_t n2 = r + k - 1;                            /* r + k - 1 */
    seq_t bin2 = binomial((idx_t)n2, (idx_t)k);      /* C(r+k-1, k) */

    /* Return the product */
    return bin1 * bin2;
}
