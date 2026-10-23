/*
 * A120854  Matrix log of A117939, read by rows, consisting only of 0's, 3's
 *          and signed 2's.
 * https://oeis.org/A120854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the n-th term obtained by reading the triangle * T(n,k)
 *            (row n, column k) defined by the ternary‑fractal rule: * *
 *            T(n,k) = 0 if n <= k or * or there is more than one differing
 *            digit in the ternary * expansions of n and k. * T(n,k) = T(m,j)
 *            otherwise, where m and j are * the distinct digi...
 * Range:     i8 n <= 119, i16 n <= 119, i32 n <= 119, i64 n <= 119,
 *            i128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

static idx_t
row_index(idx_t n) /* smallest r with (r+1)(r+2)/2 > n */
{
    idx_t low = 0, high = 1;
    /* increase high until it surpasses n */
    while (high * (high + 1) / 2 <= n) {
        high <<= 1;                 /* double high */
    }
    /* binary search between low and high-1 */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

static seq_t
T(idx_t n, idx_t k)
{
    if (n <= k)
        return 0;               /* base case n <= k */

    idx_t r = n, c = k;
    idx_t m = 0, j = 0;
    int diff = 0;

    while (r > 0 || c > 0) {
        idx_t dn = r % 3;
        idx_t dk = c % 3;
        if (dn != dk) {
            ++diff;
            if (diff > 1)
                return 0;       /* more than one differing digit */
            m = dn;
            j = dk;
        }
        r /= 3;
        c /= 3;
    }

    /* exactly one differing digit, and n > k => m > j */
    if (m == 1 && j == 0)
        return (seq_t)2;
    if (m == 2 && j == 0)
        return (seq_t)3;
    if (m == 2 && j == 1)
        return (seq_t)(-2);
    return 0;   /* should not occur */
}

seq_t a(idx_t n)
{
    idx_t r = row_index(n);
    idx_t prev = r * (r + 1) / 2;   /* number of elements before row r */
    idx_t k = n - prev;             /* column index within row r */
    return T(r, k);
}
