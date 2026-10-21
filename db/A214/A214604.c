/*
 * A214604  Odd numbers by transposing the right half of A176271, triangle
 *          read by rows: T(n,k) = A176271(n - 1 + k, n), 1 <= k <= n.
 * https://oeis.org/A214604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k)^2 - r - 3*k + 1 r = smallest integer such that
 *            r*(r+1)/2 >= n (row) k = n - (r-1)*r/2 (column) This is the
 *            row‑major enumeration of the triangle T(n,k) = (n+k)^2 - n - 3*k
 *            + 1
 * Range:     u8 n <= 43, u16 n <= 8383, u32 n <= 9909, u64 n <= 9909,
 *            u128 n <= 9909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, return 0 as sentinel */

    /* Find row r by incremental search (sufficient for n <= ~1e8). */
    idx_t r = 1;
    while ( ( (__uint128_t)r * (r + 1) ) / 2 < n )
        ++r;

    /* previous triangular sum */
    idx_t prev_tri = ( (__uint128_t)(r - 1) * r ) / 2;
    idx_t k = n - prev_tri;               /* column index (1‑based) */

    idx_t sum = r + k;
    __uint128_t val = (__uint128_t)sum * sum;  /* (r+k)^2   */
    val -= r;                                 /* minus r   */
    val -= (__uint128_t)3 * k;                 /* minus 3k  */
    val += 1;                                 /* plus 1    */
    return (seq_t)val;
}
