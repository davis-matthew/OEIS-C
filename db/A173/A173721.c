/*
 * A173721  Partial sums of A056833.
 * https://oeis.org/A173721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + n + 21) / 42)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1613, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A173721: partial sums of round(k^2/7). */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t t  = (seq_t)n;
    seq_t t2 = t * t;        /* n^2  */
    seq_t t3 = t2 * t;       /* n^3  */
    seq_t numer = 2 * t3 + 3 * t2 + t + 21; /* 2n^3 + 3n^2 + n + 21 */
    return numer / 42;                /* floor division for non‑negative integers */
}
