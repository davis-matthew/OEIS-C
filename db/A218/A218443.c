/*
 * A218443  a(n) = Sum_{k=0..n} floor(n/(3k+2)).
 * https://oeis.org/A218443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(n/(3k+2))
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t denom = 2;            /* 3*k + 2 for k = 0 */
    seq_t sum = 0;

    while (denom <= n) {
        sum += (seq_t)(n / denom);   /* floor division of idx_t values */
        ++k;
        denom = 3 * k + 2;           /* next denominator */
    }
    return sum;
}
