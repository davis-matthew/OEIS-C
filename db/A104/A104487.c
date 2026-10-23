/*
 * A104487  a(n+3) = 6a(n+2) - 10a(n+1) + 3a(n); a(0) = 1, a(1) = 4, a(2) =
 *          14.
 * https://oeis.org/A104487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 6*a(n+2) - 10*a(n+1) + 3*a(n); a(0) = 1, a(1) = 4, a(2)
 *            = 14
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n‑th term of OEIS A104487. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    seq_t s0 = (seq_t)1;    /* a(n-3) */
    seq_t s1 = (seq_t)4;    /* a(n-2) */
    seq_t s2 = (seq_t)14;   /* a(n-1) */
    seq_t s3;

    /* Iterate from 3 up to n, computing a(i) using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        /* In C, seq_t is an unsigned integer type, so the operations below
         * are performed in that type.  In C++ where seq_t is an
         * arbitrary‑precision integer class, the operators +, -, * are
         * overloaded accordingly. */
        s3 = (seq_t)6 * s2 - (seq_t)10 * s1 + (seq_t)3 * s0;
        s0 = s1;
        s1 = s2;
        s2 = s3;
    }
    return s2;
}
