/*
 * A152166  a(2*n) = 2^n; a(2*n+1) = -(2^(n+1)).
 * https://oeis.org/A152166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2*n) = 2^n; a(2*n+1) = -(2^(n+1))
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 61, i64 n <= 125,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* For uint64_t type of idx_t */

/* Computes the n-th term of OEIS A152166.
 * a(0) = 1, a(1) = -2, a(2) = 2, a(3) = -4, ...
 * The function uses left–shifts on seq_t, which is defined
 * in the build system as any signed integer type or an
 * arbitrary-precision integer class.  For the fixed‑size
 * integer types this operation is safe for all n that
 * yield values that fit in the type.  For arbitrary‑precision
 * types the shift is defined for any non‑negative shift.
 */
seq_t a(idx_t n) {
    idx_t m = n >> 1;          /* floor(n/2) */
    if ((n & 1U) == 0U) {      /* n is even: a = 2^m */
        return ((seq_t)1 << m);
    } else {                   /* n is odd: a = -2^(m+1) */
        return -((seq_t)1 << (m + 1));
    }
}
