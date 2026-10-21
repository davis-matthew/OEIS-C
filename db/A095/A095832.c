/*
 * A095832  Triangle read by rows: T(n,k) = (n-k+1)*(n-k), n>=1, 1<=k<=n.
 * https://oeis.org/A095832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r−k+1)*(r−k) where * r = least integer with r(r+1)/2 ≥
 *            n, * k = n − r(r−1)/2 (column of n in row r). * The function
 *            computes the triangle entry T(r,k) = (r−k+1)*(r−k)
 * Range:     u8 n <= 136, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)            /* not used (offset = 1) but safe */
        return (seq_t)0;

    idx_t sum = 0u;         /* cumulative number of terms so far      */
    idx_t r   = 0u;         /* current row being examined             */

    /* Find the row r that contains the n-th element   */
    while (1) {
        ++r;                /* move to next row                      */
        sum += r;           /* add the length of this row            */
        if (sum >= n)       /* row found                              */
            break;
    }

    idx_t prev = sum - r;   /* number of terms before row r           */
    idx_t k    = n - prev;  /* column index within row r (1‑based)   */
    idx_t m    = r - k + 1; /* m = r−k+1  →  T(r,k) = m*(m−1)          */

    return (seq_t)m * (seq_t)(m - 1);
}
