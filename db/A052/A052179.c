/*
 * A052179  Triangle of numbers arising in enumeration of walks on cubic
 *          lattice.
 * https://oeis.org/A052179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, tabl, easy, nice
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2 T satisfies: T(0,0)=1 T(n,0)=4*T(n-1,0)+T(n-1,1)
 *            T(n,k)=T(n-1,k-1)+4*T(n-1,k)+T(n-1,k+1) (1 ≤ k ≤ n)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 104, u64 n <= 377,
 *            u128 n <= 1378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal cache of triangle rows.
   rows[i] points to an array of length i+1 containing T(i,0..i).  */
static seq_t **rows = NULL;          /* array of pointers to rows      */
static size_t max_row = 0;           /* highest row index currently stored */

/* Ensure that rows[0..up_to] are allocated and computed.           */
static void ensure_rows(size_t up_to)
{
    if (up_to < max_row) return;

    /* Expand the rows pointer array if needed */
    if (rows == NULL) {
        rows = (seq_t **)malloc((up_to + 1) * sizeof(seq_t *));
    } else {
        rows = (seq_t **)realloc(rows, (up_to + 1) * sizeof(seq_t *));
    }

    /* Fill in new rows from (max_row) to up_to */
    for (size_t r = max_row; r <= up_to; ++r) {
        size_t len = r + 1;
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (r == 0) {                     /* base case */
            curr[0] = (seq_t)1;
        } else {
            seq_t *prev = rows[r - 1];
            size_t prev_len = r;
            for (size_t j = 0; j <= r; ++j) {
                seq_t term = 0;
                seq_t a = 0, b = 0, c = 0;
                if (j == 0) {              /* special case for k=0 */
                    seq_t four_prev0 = prev[0];
                    four_prev0 += prev[0];
                    four_prev0 += prev[0];
                    four_prev0 += prev[0]; /* 4 * prev[0] */
                    term = four_prev0;
                    if (prev_len > 1) {
                        term += prev[1];
                    }
                } else {                   /* general recurrence */
                    if (j - 1 < prev_len) a = prev[j - 1];
                    if (j < prev_len)     b = prev[j];
                    if (j + 1 < prev_len) c = prev[j + 1];

                    seq_t four_b = b;
                    four_b += b;
                    four_b += b;
                    four_b += b;           /* 4 * b */

                    term = a + four_b + c;
                }
                curr[j] = term;
            }
        }
        rows[r] = curr;
    }
    max_row = up_to;
}

/* Return the n-th term of A052179.                              */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    size_t r = 0;
    size_t sum = 0;
    while (1) {
        size_t next = r + 1;
        size_t next_sum = next * (next + 1) / 2;
        if (next_sum > n) break;
        r = next;
        sum = next_sum;
    }

    /* Ensure we have computed the necessary rows. */
    ensure_rows(r);

    /* Offset within the row. */
    size_t k = (size_t)n - sum;

    return rows[r][k];
}
