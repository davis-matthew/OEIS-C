/*
 * A140071  Triangle read by rows: iterates of X * [1,0,0,0,...]; where X = an
 *          infinite lower bidiagonal matrix with [3,1,3,1,3,1...] in the main
 *          diagonal and [1,1,1,...] in the subdiagonal.
 * https://oeis.org/A140071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle T(r,k) * defined by
 *            T(1,1)=1, T(2,1)=3, T(2,2)=1 and *
 *            T(r,k)=4·T(r-1,k)-3·T(r-2,k)+T(r-2,k-2) for r≥3, * where
 *            out‑of‑range indices are treated as 0. The triangle * is read by
 *            rows, so a(n) = T(r,k) where * r = smallest integer with
 *            r(r+1)/2 ≥ n * k = n – r(r...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *alloc_row(int len)
/* allocate array with indices 1..len (len≥1) */
{
    return (seq_t *)malloc((len + 1) * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;            /* column within the row */

    /* Base rows */
    seq_t *row1 = alloc_row(1);         /* r = 1 */
    row1[1] = (seq_t)1;
    if (r == 1) {
        seq_t res = row1[1];
        free(row1);
        return res;
    }

    seq_t *row2 = alloc_row(2);         /* r = 2 */
    row2[1] = (seq_t)3;
    row2[2] = (seq_t)1;
    if (r == 2) {
        seq_t res = row2[(int)k];
        free(row1);
        free(row2);
        return res;
    }

    /* Iteratively build rows up to r */
    seq_t *prev2 = row1;                /* row r-2 */
    seq_t *prev1 = row2;                /* row r-1 */
    seq_t *curr = NULL;

    for (idx_t i = 3; i <= r; ++i) {
        curr = alloc_row((int)i);
        for (idx_t j = 1; j <= i; ++j) {
            seq_t val = 0;
            if (j <= i - 1)                     /* 4 * T(i-1,j) */
                val += (seq_t)4 * prev1[(int)j];
            if (j <= i - 2)                     /* -3 * T(i-2,j) */
                val -= (seq_t)3 * prev2[(int)j];
            if (j >= 3)                          /* + T(i-2,j-2) */
                val += prev2[(int)(j - 2)];
            curr[(int)j] = val;
        }
        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    seq_t result = prev1[(int)k];
    free(prev2);
    free(prev1);
    return result;
}
