/*
 * A098737  Triangle read by rows: number of triangles formed by lines from
 *          two vertices of a triangle to the opposite side that segment the
 *          opposite sides into m and n segments. Since f(m,n) = f(n,m), it
 *          suffices to give the results in a triangular table.
 * https://oeis.org/A098737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by locating the row m that contains the index n
 *            in the triangle; row m has m entries. Let m = smallest integer
 *            such that m(m+1)/2 >= n. Let k = n - (m-1)m/2 (1 <= k <= m).
 *            Then a(n) = f(m,k) = m * k * (m + k) / 2
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row number m by binary search over 1..n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sum = (__uint128_t)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* offset of the first element of row m */
    __uint128_t offset = (__uint128_t)(m - 1) * m / 2;
    idx_t k = (idx_t)(n - offset); /* column index in row m */

    /* compute the value a(n) = m * k * (m + k) / 2 */
    seq_t result = (seq_t)m;
    result *= (seq_t)k;
    result *= (seq_t)(m + k);
    result /= 2;
    return result;
}
