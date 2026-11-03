/*
 * A190347  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(5/3), t=sqrt(3/5).
 * https://oeis.org/A190347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sqrt(5/3)⌋ + ⌊n*sqrt(3/5)⌋
 * Range:     u8 n <= 6, u16 n <= 390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Return the largest integer k such that a*k^2 <= b*n^2.
 * Used to compute ⌊n*sqrt(b/a)⌋ exactly with integer arithmetic.
 */
static inline seq_t floor_sqrt_coeff(seq_t n, seq_t a, seq_t b)
{
    seq_t low  = 0;
    seq_t high = n;            /* initial bound – will be increased if necessary */
    const seq_t one = 1;

    /* Expand high until the inequality fails */
    while (1) {
        seq_t next = high + one;
        seq_t rhs  = a * next * next;      /* a*(k+1)^2   */
        seq_t lhs  = b * n * n;            /* b*n^2       */
        if (rhs <= lhs)
            high = next;
        else
            break;
    }

    /* Binary search for the maximal k */
    while (low <= high) {
        seq_t mid = (low + high) >> 1;     /* average */
        seq_t rhs = a * mid * mid;
        seq_t lhs = b * n * n;
        if (rhs <= lhs) {
            low = mid + one;
        } else {
            if (mid == 0) return 0;
            high = mid - one;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for n < 1 */

    seq_t ns   = (seq_t)n;
    seq_t k1   = floor_sqrt_coeff(ns, (seq_t)3, (seq_t)5); /* ⌊n*sqrt(5/3)⌋ */
    seq_t k2   = floor_sqrt_coeff(ns, (seq_t)5, (seq_t)3); /* ⌊n*sqrt(3/5)⌋ */
    return ns + k1 + k2;
}
