/*
 * A232395  (ceiling(sqrt(n^3 + n^2 + n + 1)))^2 - (n^3 + n^2 + n + 1).
 * https://oeis.org/A232395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (ceiling(sqrt(n^3 + n^2 + n + 1)))^2 - (n^3 + n^2 + n +
 *            1)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 605, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root (floor) using binary search; seq_t is an unsigned integer type */
static seq_t int_isqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t m  = n3 + n2 + nn + 1;          /* n^3 + n^2 + n + 1 */
    seq_t k  = int_isqrt(m);              /* floor(sqrt(m)) */
    if (k * k < m)                        /* ceil sqrt */
        ++k;
    return k * k - m;                     /* difference to nearest square */
}
