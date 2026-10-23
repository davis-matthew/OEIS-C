/*
 * A153272  Triangle T(n, k) = Product_{j=0..k} (j*n + prime(m)), with T(n, 0)
 *          = prime(m) and m = 4, read by rows.
 * https://oeis.org/A153272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, k) = \prod_{j=0}^k (j*n + 7), a(n) is the row‑major
 *            enumeration of this triangle (m = 4)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 34, u64 n <= 76, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the first element of row r
       has index S(r-1) = r*(r+1)/2 and the last element of row r
       has index S(r)-1 = (r+1)*(r+2)/2 - 1.  */
    idx_t r = 0;
    idx_t cum = 1;                      /* S(0) = 1 */
    while (cum <= n) {
        ++r;
        cum = (r + 1) * (r + 2) / 2;     /* S(r) */
    }

    idx_t prev_cum = r * (r + 1) / 2;    /* S(r-1) */
    idx_t k = n - prev_cum;              /* position in row r */

    /* Compute T(r, k) = product_{j=0..k} (j*r + 7) */
    seq_t result = 1;
    for (idx_t j = 0; j <= k; ++j) {
        result *= ((seq_t)j * (seq_t)r + 7);
    }
    return result;
}
