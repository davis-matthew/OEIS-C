/*
 * A051231  Triangle read by rows: T(n, m) = S1(n, m)*9^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 28, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the n‑th term of the triangle T(n,m) defined by
 *   T(1,1) = 1,
 *   T(n,m) = T(n-1,m-1) - 9·(n-1)·T(n-1,m)   for n>1,
 *   T(n,0)=T(n,n)=0 for n>1.
 * a(n) is obtained from the triangle by reading it row‑wise.
 */

seq_t a(idx_t n)
{
    /* Find the row `r` such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;
    /* Column `m` within that row */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t m = n - prev_tri;          /* 1‑based column */

    /* The maximum row needed for n up to about 10000 is < 200. */
    #define MAX_R 200
    seq_t prev[MAX_R + 1];
    seq_t cur [MAX_R + 1];

    /* Build the triangle row by row. */
    for (idx_t row = 1; row <= r; ++row) {
        if (row == 1) {
            prev[1] = 1;            /* T(1,1) */
        } else {
            seq_t mult = (seq_t)9 * (row - 1);   /* 9*(row-1) */
            /* m = 1 term */
            cur[1] = -(mult * prev[1]);

            /* m >= 2 terms */
            for (idx_t col = 2; col <= row; ++col) {
                seq_t valPrevM1 = prev[col - 1];
                seq_t valPrevM  = (col <= row - 1) ? prev[col] : 0;
                cur[col] = valPrevM1 - (mult * valPrevM);
            }

            /* Copy current row into prev for next iteration */
            for (idx_t col = 1; col <= row; ++col) prev[col] = cur[col];
        }
    }

    return prev[m];
}
