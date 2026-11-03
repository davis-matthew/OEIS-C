/*
 * A070929  Smallest integer >= 0 of the form x^2 - n^3.
 * https://oeis.org/A070929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(n^(3/2))^2 - n^3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-01)
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
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;          /* n³ */

    seq_t sq = isqrt_seq(n3);    /* floor sqrt(n³) */
    seq_t sq_sq = sq * sq;
    if (sq_sq < n3) {            /* need ceil */
        sq = sq + 1;
        sq_sq = sq * sq;
    }
    return sq_sq - n3;
}
