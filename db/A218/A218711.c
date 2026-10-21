/*
 * A218711  Number of nonnegative solutions to x^2 + y^2 + z^2 < n^2.
 * https://oeis.org/A218711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions of x^2 + y^2 +
 *            z^2 < n^2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ 2^64‑1. */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t op = n;
    idx_t res = 0;
    idx_t one = (idx_t)1 << 62;            /* high power of four ≤ 2^64 */
    while (one > op) one >>= 2;
    for (; one != 0; one >>= 2) {
        if (op >= res + one) {
            op -= res + one;
            res = (res >> 1) + one;
        } else {
            res >>= 1;
        }
    }
    return res;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    seq_t result = 0;

    /* n^2 fits easily in idx_t for the expected n (≤ 10 000). */
    idx_t n_sq = n * n;

    for (idx_t x = 0; x * x < n_sq; ++x) {
        idx_t xsq = x * x;
        /* Remaining value for y: x^2 + y^2 < n^2  →  y^2 < n^2 – x^2. */
        idx_t y_rem = n_sq - xsq;              /* > 0 because xsq < n_sq */
        idx_t y_max = _floor_isqrt_u64(y_rem - 1); /* floor(sqrt(y_rem-1)) */
        for (idx_t y = 0; y <= y_max; ++y) {
            idx_t ysq = y * y;
            /* Remaining value for z: x^2 + y^2 + z^2 < n^2  →  z^2 < n^2 – xsq – ysq. */
            idx_t rem = n_sq - xsq - ysq;            /* ≥ 1 for y ≤ y_max */
            idx_t z_max = _floor_isqrt_u64(rem - 1); /* floor(sqrt(rem-1)) */
            result += (seq_t)(z_max + 1);            /* z runs from 0 to z_max inclusive */
        }
    }
    return result;
}
