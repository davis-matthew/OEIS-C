/*
 * A077606  Left differencing matrix, D, by antidiagonals.
 * https://oeis.org/A077606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 1 if 2n-1 is an odd perfect square, a(n) = -1 if n/2 is a
 *            perfect square, otherwise a(n) = 0
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer (floor).   */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;           /* highest power of four ≤ 2⁶⁴ */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */

    /* case -1 : n must be even and n/2 a perfect square */
    if ((n & 1ULL) == 0) {
        idx_t m = n >> 1;               /* n/2 */
        idx_t s = isqrt_u64(m);
        if (s * s == m) return (seq_t)-1;
    }

    /* case  1 : 2n-1 must be an odd perfect square */
    idx_t v = (n << 1) - 1;              /* 2n-1, safe for n < 2⁶³ */
    idx_t t = isqrt_u64(v);
    if (t * t == v && (t & 1ULL) == 1ULL)
        return (seq_t)1;

    return 0;
}
