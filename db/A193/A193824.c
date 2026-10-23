/*
 * A193824  Mirror of the triangle A193823.
 * https://oeis.org/A193824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r - (n - r(r+1)/2)), where w(0,0)=1, w(n,0)=1,
 *            w(n,n)=3^(n-1), and for 1 ≤ k ≤ n-1 :
 *            w(n,k)=w(n‑1,k)+2·w(n‑1,k‑1)
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the value of the sequence at index n. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((__int128)(r + 1) * (r + 2) / 2 <= n)
        r++;

    /* The term resides in row r, position pos_in_row in the normal order. */
    idx_t pos_in_row = n - r * (r + 1) / 2;   /* 0‑based from left end in the normal row */
    idx_t k = r - pos_in_row;                 /* index in the triangle (reversed row) */

    if (r == 0)          /* the first term */
        return (seq_t)1;

    /* Build rows up to r using only two dynamically allocated rows. */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));   /* row 0 */
    prev[0] = (seq_t)1;
    seq_t pow3_val = (seq_t)1;   /* 3^(i-1) for the next row i */

    for (idx_t i = 1; i <= r; ++i) {
        seq_t *curr = (seq_t *)malloc((i + 1) * sizeof(seq_t));

        /* Borders of the triangle. */
        curr[0] = (seq_t)1;                 /* w(i,0) = 1 */
        curr[i] = pow3_val;                 /* w(i,i) = 3^(i-1) */

        /* Interior values. */
        for (idx_t j = 1; j < i; ++j)
            curr[j] = prev[j] + (seq_t)2 * prev[j - 1];

        pow3_val = pow3_val * (seq_t)3;       /* next 3^(i) for the following row */

        free(prev);
        prev = curr;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
