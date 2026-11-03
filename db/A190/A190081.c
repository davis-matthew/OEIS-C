/*
 * A190081  n + [n*r/t] + [n*s/t]; r=1, s=cos(Pi/5), t=sec(Pi/5).
 * https://oeis.org/A190081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*cos(pi/5)] + [n*(cos(pi/5))^2]
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 9366, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* compute floor(n*sqrt5) as floor(sqrt(5*n*n)) */
    seq_t t = isqrt_seq((seq_t)5 * N * N);

    /* floor(n*cos(pi/5)) = floor((N + t)/4)   */
    seq_t first  = (N + t) / 4;
    /* floor(n*cos^2(pi/5)) = floor((3*N + t)/8) */
    seq_t second = ((N * 3) + t) / 8;

    return N + first + second;
}
