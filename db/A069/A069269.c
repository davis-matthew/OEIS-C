/*
 * A069269  Second level generalization of Catalan triangle (0th level is
 *          Pascal's triangle A007318; first level is Catalan triangle
 *          A009766; 3rd level is A069270).
 * https://oeis.org/A069269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069269
   The sequence is obtained by reading the rows of the second‑level
   generalised Catalan triangle (m = 2) row by row.  
   T(n,k) = C(n+2k , k) * (n-k+1) / (n+k+1)
   where 0 ≤ k ≤ n.  a(0) = T(0,0) = 1, a(1) = T(1,0), a(2) = T(1,1), … */
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static idx_t row_for_idx(idx_t idx)
{
    /* Find r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2 */
    if (idx == 0) return 0;
    /* initial estimate using double arithmetic (safe for idx ≤ 1e12) */
    double d = sqrt((long double)8 * idx + 1);
    idx_t r = (idx_t)((d - 3.0L) / 2.0L);
    /* Correct possible off‑by‑one errors due to truncation */
    while ((r + 1) * (r + 2) / 2 <= idx) r++;
    while (r * (r + 1) / 2 > idx) r--;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the (r,k) pair corresponding to the nth element in the
       concatenation of all rows of the triangle. */
    idx_t r = row_for_idx(n);                  /* row number  */
    idx_t start = r * (r + 1) / 2;             /* first index of row r  */
    idx_t k = n - start;                       /* column in row r      */

    /* Compute T(r,k) = C(r+2k , k) * (r-k+1) / (r+k+1) using big ints. */
    seq_t bin = binomial(r + 2 * k, k);
    seq_t num = bin * (seq_t)(r - k + 1);
    seq_t den = (seq_t)(r + k + 1);
    return num / den;
}
