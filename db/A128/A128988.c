/*
 * A128988  a(n) = (n^3 - n^2)*5^n.
 * https://oeis.org/A128988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*5^n = n^2*(n-1)*5^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 876 (2026-11-01)
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
    /* Offset starts at 1; for n < 1 the value is undefined, but
       return 0 for n==0 to avoid UB in caller. */
    if (n == 0) return 0;

    /* Compute the power of 5: 5^n */
    seq_t pow5 = power5(n);

    /* Compute n^2 * (n-1) */
    seq_t term = (seq_t)n * (seq_t)n;   /* n^2 */
    term = term * (seq_t)(n - 1);       /* n^2*(n-1) */

    /* Final result */
    return term * pow5;
}
