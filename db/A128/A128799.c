/*
 * A128799  a(n) = n*(n-1)*5^n.
 * https://oeis.org/A128799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 894 (2026-11-01)
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
    if (n <= 1u)
        return 0;

    seq_t nn      = (seq_t)n;
    seq_t nminus1 = (seq_t)(n - 1u);
    seq_t pow5    = power5(n);        /* 5^n */

    return nn * nminus1 * pow5;
}
