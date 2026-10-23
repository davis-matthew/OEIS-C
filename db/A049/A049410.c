/*
 * A049410  A triangle of numbers related to triangle A049325.
 * https://oeis.org/A049410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by flattening the lower‑triangular array a(n,m)
 *            defined by a(1,1)=1, a(n,m)=0 for n<m or m=0,
 *            a(n,m)=(4*m-n+1)*a(n-1,m)+a(n-1,m-1) for n>=m>=1
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

/* Return the n-th term of OEIS A049410 (offset 1).  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* the sequence is defined only for n≥1 */

    /* Determine the row r and column c of the flattened index n.
       total(r) = r*(r+1)/2 is the number of terms up to row r. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;

    idx_t prev_total = (r - 1) * r / 2;   /* terms before row r */
    idx_t c = n - prev_total;             /* 1‑based column within row r */

    /* Build the triangle row by row only keeping the current and previous rows. */
    size_t prev_len = 1;
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (prev_len + 1));
    if (!prev) return (seq_t)0;           /* allocation failure */
    prev[1] = (seq_t)1;                   /* a(1,1) = 1 */

    if (r == 1) {                         /* n = 1 → answer already computed */
        seq_t result = prev[1];
        free(prev);
        return result;
    }

    for (idx_t row = 2; row <= r; ++row) {
        size_t curr_len = (size_t)row;
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (curr_len + 1));
        if (!curr) { free(prev); return (seq_t)0; }

        for (size_t m = 1; m <= curr_len; ++m) {
            seq_t term1 = (m <= prev_len) ? prev[m] : (seq_t)0;
            seq_t term2 = (m > 1) ? prev[m - 1] : (seq_t)0;
            seq_t coef   = (seq_t)((4 * m) - row + 1);
            curr[m] = coef * term1 + term2;
        }

        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Retrieve the desired entry and clean up. */
    seq_t result = prev[(size_t)c];
    free(prev);
    return result;
}
