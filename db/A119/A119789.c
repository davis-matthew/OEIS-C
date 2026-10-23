/*
 * A119789  T(n, k) = floor(log_{goldenratio}(Fibonacci(n)*Fibonacci(k))),
 *          with T(n, k) = 0 for n < 3, T(n, 0) = n-2 for n > 2, triangle read
 *          by rows.
 * https://oeis.org/A119789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 0 for n<3; else if k<=1: T(n,k) = n-2; else: T(n,k) = n
 *            + k - 4. a(n) is obtained from the triangle by rows: a(n) =
 *            T(r,k) where r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0, k;
    if (n != 0) {
        /* Find an upper bound for r. */
        idx_t high = 1;
        while ((unsigned __int128)high * (high + 1) / 2 <= n)
            high <<= 1;
        /* Binary search for the exact r. */
        idx_t low = 0;
        while (low < high) {
            idx_t mid = low + (high - low + 1) / 2;
            unsigned __int128 mid_val = (unsigned __int128)mid * (mid + 1) / 2;
            if (mid_val <= n)
                low = mid;
            else
                high = mid - 1;
        }
        r = low;
    }

    /* k is the offset within the row. */
    k = n - (unsigned __int128)r * (r + 1) / 2;

    if (r < 3)
        return (seq_t)0;

    if (k <= 1)
        return (seq_t)(r - 2);   /* T(r,0) or T(r,1) */
    else
        return (seq_t)(r + k - 4);   /* T(r,k) for k>=2 */
}
