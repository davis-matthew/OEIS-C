/*
 * A154990  Triangle read by rows. Main diagonal is positive. The rest of the
 *          terms are negative.
 * https://oeis.org/A154990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = 1 if n is a triangular number, otherwise -1
 * Range:     i8 n <= 20, i16 n <= 465, i32 n <= 465, i64 n <= 465,
 *            i128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit values.
   Returns floor(sqrt(x)). */
static idx_t isqrt_u64(idx_t x) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;          /* sqrt(2^64-1) < 2^32 */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* OEIS offset is 1; the function is defined for n ≥ 1. */
    seq_t val = (seq_t)8 * (seq_t)n + 1;     /* 8·n + 1 */
    idx_t r = isqrt_u64((idx_t)val);        /* floor(sqrt(val)) */
    seq_t sq = (seq_t)r * (seq_t)r;
    return (sq == val) ? 1 : -1;
}
