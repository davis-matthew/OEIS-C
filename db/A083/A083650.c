/*
 * A083650  Expansion of f(-x, x^3) * phi(x^2) in powers of x where phi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A083650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k≥0, k(k+1)/2 ≤ n} (-1)^(k+[k/4]) * ( n==k(k+1)/2 ? 1
 *            : ((n-k(k+1)/2) even && ((n-k(k+1)/2)/2 is a perfect square) ? 2
 *            : 0 )
 * Range:     i8 n <= 49, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root, floor. Works for any seq_t that supports arithmetic. */
static inline seq_t isqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor sqrt */
}

/* Test whether a non‑negative seq_t is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t r = isqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    const seq_t nn = (seq_t)n;          /* convert once */
    for (idx_t k = 0; ; ++k) {
        const seq_t kk = (seq_t)k;
        const seq_t T  = (kk * (kk + 1)) / 2;   /* k(k+1)/2 */
        if (T > nn) break;
        /* Sign: (-1)^(k + ⌊k/4⌋) */
        const seq_t parity = ((kk + (kk / 4)) & 1);
        const seq_t sign   = parity ? -1 : 1;
        const seq_t rem   = nn - T;          /* n - T_k */
        if (rem == 0) {
            sum += sign;                    /* coefficient 1 when m = 0 */
        } else if ((rem & 1) == 0) {         /* rem even */
            const seq_t half = rem >> 1;     /* (n - T_k)/2 */
            if (is_square(half)) {
                sum += sign * 2;             /* coefficient 2 for m ≠ 0 */
            }
        }
    }
    return sum;
}
