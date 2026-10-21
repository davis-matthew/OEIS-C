/*
 * A061650  a(n) = n*20^(n-1).
 * https://oeis.org/A061650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*20^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n<1 */

    /* fast exponentiation: compute 20^(n-1) in seq_t */
    seq_t base  = 20;               /* base of the power */
    seq_t power = 1;                /* accumulates the result */
    idx_t exp   = n - 1;            /* exponent */

    while (exp) {
        if (exp & 1) power *= base;
        exp >>= 1;
        if (exp) base *= base;
    }

    /* multiply by n (cast to seq_t) */
    return (seq_t)n * power;
}
