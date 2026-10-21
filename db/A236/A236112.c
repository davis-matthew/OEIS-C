/*
 * A236112  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          k+1 copies of the squares in nondecreasing order, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A236112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the triangle read by rows. The
 *            triangle entry in row r (r>=1) and column k (k>=1) is: T(r,k) =
 *            ( floor( (r - k(k+1)/2) / (k+1) ) )^2, provided k(k+1)/2 <= r.
 *            The sequence a(n) lists T(r,k) for r=1,2,3,... and k=1..kmax(r),
 *            where kmax(r) is the largest k such that k(k+1)/...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, but return 0 for safety */
        return 0;

    idx_t total = 0;      /* number of sequence terms generated so far */

    for (idx_t row = 1; ; ++row) {
        /* find maximum column index k such that k(k+1)/2 <= row */
        idx_t kmax = 0;
        while ((kmax + 1) * (kmax + 2) / 2 <= row)
            ++kmax;

        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t start = k * (k + 1) / 2;        /* first row of column k */
            idx_t d     = row - start;            /* offset within the column   */
            idx_t group = d / (k + 1);            /* which square in this group */
            seq_t val   = (seq_t)group * (seq_t)group;

            ++total;
            if (total == n)
                return val;
        }
    }
}
