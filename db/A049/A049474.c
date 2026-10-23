/*
 * A049474  a(n) = ceiling(n/sqrt(2)).
 * https://oeis.org/A049474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / sqrt(2))
 * Range:     u8 n <= 360, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = ceil(n/sqrt(2)) for n >= 0.
   Uses a binary search on m such that 2*m*m >= n*n.
   For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = 0;
    idx_t high = n;          /* ceil(n/sqrt(2)) < n for n > 0 */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* Use 128‑bit arithmetic to avoid overflow */
        __int128 lhs = (__int128)mid * mid * 2;
        __int128 rhs = (__int128)n * n;

        if (lhs >= rhs)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
