/*
 * A204854  G.f.: Sum_{n>=0} x^n * Product_{k=1..n} (1 - x^k) / (1 + x^k).
 * https://oeis.org/A204854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (1 - 2 * floor_sqrt(n))
 * Range:     i8 n <= 4224, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)), for n <= 2^64-1.
 * Uses the Newton‑Raphson method, always converging to the
 * correct integer floor value without overflow. */
static inline idx_t floor_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + (n / x)) >> 1;
    while (y < x) {
        x = y;
        y = (x + (n / x)) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt(n);          /* floor(sqrt(n)) */
    seq_t val = (seq_t)(1 - 2 * (seq_t)k);  /* 1 - 2*k fits in seq_t */
    if (n & 1ULL) val = -val;          /* multiply by (-1)^n */
    return val;
}
