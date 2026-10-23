/*
 * A037012  Triangle read by rows; row 0 is 0; the n-th row for n>0 contains
 *          the coefficients in the expansion of (1-x)*(1+x)^(n-1).
 * https://oeis.org/A037012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the coefficient in the triangle defined by /*
 *            (1-x)*(1+x)^(row-1). The triangle satisfies: /* T(r,0)=1,
 *            T(r,r)=-1, T(r,k)=T(r-1,k-1)+T(r-1,k) (1≤k≤r-1). /* The function
 *            returns the n‑th term of the flattened triangle
 * Range:     i8 n <= 81, i16 n <= 237, i32 n <= 754, i64 n <= 2583,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The very first term is 0 */
    if (n == 0)
        return 0;

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Offset of the row in the flattened sequence */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column within row r */

    /* Handle the first two rows directly */
    if (r == 1)
        return (k == 0) ? (seq_t)1 : (seq_t)-1;

    /* Build rows 2 .. r using the recurrence.
       For n≤10000 the maximum r is about 141, so a fixed-size array is sufficient. */
    const idx_t MAX_R = 200;      /* safely > all needed row indices */
    seq_t prev[MAX_R];
    seq_t cur[MAX_R];

    /* Row 1 */
    prev[0] = 1;
    prev[1] = -1;

    for (idx_t i = 2; i <= r; ++i) {
        cur[0] = 1;                      /* T(i,0) = 1 */
        for (idx_t j = 1; j < i; ++j) {
            cur[j] = prev[j - 1] + prev[j];
        }
        cur[i] = -1;                     /* T(i,i) = -1 */

        /* Copy current row to prev for next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = cur[j];
    }

    /* Return the requested element in the final row */
    return prev[k];
}
