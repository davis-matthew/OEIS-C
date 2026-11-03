/*
 * A082532  a(n) = n^2 - 2*floor(n/sqrt(2))^2.
 * https://oeis.org/A082532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - 2*floor(n/sqrt(2))^2
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 8385, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9911 (2026-11-01)
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

seq_t a(idx_t n) {
    /* Convert index to seq_t once */
    seq_t ns = (seq_t)n;
    /* Compute n^2 */
    seq_t n2 = ns * ns;
    /* floor(n^2 / 2) */
    seq_t half = n2 / 2;
    /* floor(sqrt( floor(n^2 / 2) )) */
    seq_t k = isqrt_seq(half);
    /* Return n^2 - 2*k^2 */
    return n2 - (seq_t)2 * k * k;
}
