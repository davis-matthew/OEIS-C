/*
 * A143150  A triangle sequence based on the Folium of Descartes: x^3 + y^3 -
 *          3*a*x*y.
 * https://oeis.org/A143150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row(n), col(n)) where * row(n) = least k with k*(k+1)/2
 *            >= n, * col(n) = n - row(n)*(row(n)-1)/2, * T(n,m) = n^3 + m^3 -
 *            3*n*m. * The sequence is defined by the triangle A143150
 * Range:     i8 n <= 13, i16 n <= 350, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1: not defined, return 0 for safety */
        return 0;

    /* Find the row index k such that k*(k+1)/2 >= n. */
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until its triangular number reaches or exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;                      /* high *= 2 */

    /* Binary search for the smallest k with k*(k+1)/2 >= n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t row = low;

    /* Compute the previous triangular number to get the column index. */
    idx_t prev_triang = (row > 1) ? ((unsigned __int128)(row - 1) * row / 2) : 0;
    idx_t col = n - prev_triang;

    /* Compute T(row, col) in seq_t arithmetic. */
    seq_t r = (seq_t)row;
    seq_t c = (seq_t)col;

    seq_t r3 = r * r * r;
    seq_t c3 = c * c * c;
    seq_t res = r3 + c3 - 3 * r * c;

    return res;
}
