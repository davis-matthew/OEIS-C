/*
 * A130304  Triangle, (1,1,0,0,1,1,0,0,...) in every column.
 * https://oeis.org/A130304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if ((row(n) - col(n)) mod 4) < 2, otherwise 0, where
 *            row(n) is the smallest r with r(r+1)/2 ≥ n, and col(n) = n -
 *            r(r-1)/2
 * Range:     u8 n <= 45, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* not used, but harmless */

    /* binary search for r: smallest r such that r(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* previous triangular number T_{r-1} = (r-1)*r/2 */
    seq_t prevTriSeq = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t prevTri = (idx_t)prevTriSeq;

    /* column index in this row */
    idx_t c = n - prevTri;

    /* difference r - c modulo 4 */
    idx_t diff = r - c;
    diff &= 3;           /* same as diff % 4 for unsigned types */

    return (diff < 2) ? (seq_t)1 : (seq_t)0;
}
