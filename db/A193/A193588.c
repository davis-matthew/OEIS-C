/*
 * A193588  A Fibonacci triangle: T(n,k) = Fib(k+2) for 0 <= k <= n.
 * https://oeis.org/A193588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fib( (n - r(r+1)/2) + 2 ), where r = floor such that
 *            r(r+1)/2 <= n < (r+1)(r+2)/2. This concatenates the rows of the
 *            Fibonacci triangle
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row r where n lies */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= n)
        high <<= 1;                  /* exponential search for an upper bound */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t T_r = ((__int128)r * (r + 1)) / 2;
    idx_t k = n - T_r;                /* position inside the row, 0 <= k <= r */

    /* compute Fibonacci number Fib(k+2) */
    idx_t m = k + 2;                 /* the index we need in the Fibonacci sequence */
    seq_t f0 = 0, f1 = 1;            /* F0, F1 */
    if (m == 0)
        return f0;
    if (m == 1)
        return f1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    return f1;
}
