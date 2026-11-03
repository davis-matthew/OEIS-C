/*
 * A220511  a(n) = n^5 + 5*n + 5^n.
 * https://oeis.org/A220511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 5*n + 5^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t N      = (seq_t)n;            /* cast index to seq_t */
    seq_t n2     = N * N;               /* n^2 */
    seq_t n3     = n2 * N;              /* n^3 */
    seq_t n4     = n3 * N;              /* n^4 */
    seq_t n5     = n4 * N;              /* n^5 */
    seq_t fiveN  = N * (seq_t)5;        /* 5*n */
    seq_t pow5n  = power5(n);           /* 5^n, provided by utils */
    seq_t result = n5 + fiveN + pow5n;  /* sum all parts */
    return result;
}
