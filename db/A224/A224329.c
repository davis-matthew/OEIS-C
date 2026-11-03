/*
 * A224329  Number of idempotent n X n 0..4 matrices of rank n-1.
 * https://oeis.org/A224329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*5^(n-1)-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 201 (2026-11-01)
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
    if (n == 0)
        return 0;   /* just for safety, offset starts at 1 */

    seq_t pow5 = power5(n - 1);           /* 5^(n-1) */
    seq_t inner = ((seq_t)2 * pow5) - (seq_t)1;  /* 2*5^(n-1) - 1 */
    return inner * n;                     /* n*(2*5^(n-1)-1) */
}
