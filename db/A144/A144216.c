/*
 * A144216  C(m,2)+C(n,2), m>=1, n>=1: a rectangular array R read by
 *          antidiagonals.
 * https://oeis.org/A144216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(m,k) read by antidiagonals, where * R(m,k) =
 *            (m(m-1)+k(k-1))/2. The index n is converted to the * pair (m,k)
 *            as follows: let d be the smallest integer with * d(d+1)/2 ≥ n.
 *            Then let offset = n - (d-1)d/2 - 1, * m = offset + 1, k = d -
 *            offset
 * Range:     u8 n <= 276, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the diagonal d such that the first d(d+1)/2 terms reach n */
    idx_t d   = 1;
    idx_t sum = 1;          /* sum of 1..d */
    while (sum < n) {
        ++d;
        sum += d;          /* sum now = d(d+1)/2 */
    }
    /* number of terms before diagonal d */
    idx_t prev = sum - d;   /* sum of 1..(d-1) */
    /* position inside the current diagonal (0‑based) */
    idx_t offset = n - prev - 1;
    idx_t m = offset + 1;   /* row index in the array */
    idx_t k = d - offset;   /* column index in the array */

    seq_t mm = (seq_t)m;
    seq_t kk = (seq_t)k;
    return (mm * (mm - 1) + kk * (kk - 1)) / 2;
}
