/*
 * A132950  Triangle read by rows: t(n,m) = (m+1)^n*m^(n*(n-1)/2).
 * https://oeis.org/A132950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   t(n,m) = (n+1)^m * n^(m*(m-1)/2); a(n) is the triangle read by
 *            rows, offset 0
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 34, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r such that T(r) = r*(r+1)/2 <= n < T(r+1). */
static idx_t row_from_index(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Power of a seq_t base to an idx_t exponent. */
static seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t r = row_from_index(n);
    idx_t start = (__int128)r * (r + 1) / 2;
    idx_t m = n - start;

    if (m == 0)
        return (seq_t)1;   /* t(r,0) = 1 */

    seq_t part1 = pow_seq_u64((seq_t)(r + 1), m);

    __int128 e2 = (__int128)m * (m - 1) / 2;   /* m*(m-1)/2 */
    idx_t exp2 = (idx_t)e2;
    seq_t part2 = pow_seq_u64((seq_t)r, exp2);

    return part1 * part2;
}
