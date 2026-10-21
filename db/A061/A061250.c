/*
 * A061250  a(n) = (n-2)*(n-1)^n.
 * https://oeis.org/A061250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)^n (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) and a(2) are zero by definition */
    if (n <= 2)
        return 0;

    seq_t base  = (seq_t)(n - 1);   /* (n-1) */
    seq_t power = 1;                /* (n-1)^n computed by exponentiation by squaring */
    idx_t  exp  = n;

    while (exp) {
        if (exp & 1)
            power *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)(n - 2) * power;
}
