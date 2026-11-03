/*
 * A096198  Triangle read by rows: T(m,n)=A029837(m)+A029837(n), where
 *          (m,n)=(1,1); (2,1), (1,2); (3,1), (2,2), (1,3); ...
 * https://oeis.org/A096198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(m,n) = ceil(log2(m)) + ceil(log2(n)) * a(n) is the sequence
 *            obtained by reading the triangle * T row‑wise where row r
 *            contains (m,n) = (r,1),(r-1,2),…,(1,r)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t ceil_log2_idx(idx_t x)
{
    /* returns ceil(log2(x)) for x >= 1; sequence definition uses
       ceil(log2(1)) = 0, therefore the result is 0 for x <= 1.   */
    if (x <= 1) return 0;
    seq_t r = 0;
    idx_t y = x - 1;             /* use floor_log2(x-1) + 1 */
    while (y)
    {
        y >>= 1;
        r++;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find the row r such that the triangular number r*(r+1)/2 is
       the first one >= n.  Use an incremental accumulation to avoid
       overflow.                                                */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n)
    {
        r++;
        tri += r;                 /* tri = r*(r+1)/2 after the loop */
    }

    /* tri now holds the triangular number of row r.
       The previous triangular number is tri - r.                     */
    idx_t prev_tri = tri - r;

    /* Zero‑based position inside the row.                           */
    idx_t k = n - prev_tri - 1;

    /* Determine the m and n coordinates of the pair.                */
    idx_t m = r - k;         /* first coordinate          */
    idx_t nn = 1 + k;        /* second coordinate         */

    /* Compute the value of the sequence entry.                       */
    return ceil_log2_idx(m) + ceil_log2_idx(nn);
}
