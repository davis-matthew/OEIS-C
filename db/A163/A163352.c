/*
 * A163352  Triangle T(n,k) = m, 0<=k<=n, read by rows, where each decimal
 *          digit d of the integer m is the maximum of the corresponding
 *          decimal digits of n and k.
 * https://oeis.org/A163352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where row = floor((sqrt(8*n+1) - 1) / 2) (per
 *            the triangular index) col = n - row*(row+1)/2 T(x,y) = integer
 *            whose each decimal digit d is max( digit(x), digit(y) ). The
 *            implementation computes the row/col by an incremental search and
 *            evaluates T by iterating through decimal di...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t tri = 0;               /* tri = r*(r+1)/2 */
    while (tri + r + 1 <= n) {
        r++;
        tri += r;                /* tri now is r*(r+1)/2 for new r */
    }

    idx_t col = n - tri;         /* position inside the row */

    /* compute T(r, col) by digitwise maximum */
    seq_t result = 0;
    seq_t multiplier = 1;
    while (r > 0 || col > 0) {
        /* digits are between 0 and 9, so int is safe */
        int da = (int)(r % 10);
        int db = (int)(col % 10);
        int dmax = (da > db) ? da : db;
        result += (seq_t)dmax * multiplier;
        multiplier *= 10;
        r /= 10;
        col /= 10;
    }
    return result;
}
