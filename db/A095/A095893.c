/*
 * A095893  Triangle read by rows: T(n,k) = (n-k)^(n-k+1), n>=1, 1<=k<=n.
 * https://oeis.org/A095893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (m - k)^(m - k + 1) where m is the smallest integer with
 *            m*(m+1)/2 >= n, k = n - (m-1)*m/2 (the position in row m), 1 <=
 *            k <= m. This implements the triangle read by rows of A095893
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row m */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n)
        m++;

    /* number of terms before row m */
    idx_t prev = (m - 1) * m / 2;

    /* position within the row (1‑based) */
    idx_t k = n - prev;

    /* difference d = m - k, 0 <= d < m */
    seq_t d = (seq_t)(m - k);

    /* compute d^(d+1) */
    seq_t result = 1;
    seq_t base = d;
    seq_t exp  = d + 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}
