/*
 * A028724  a(n) = (1/2)*floor(n/2)*floor((n-1)/2)*floor((n-2)/2).
 * https://oeis.org/A028724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*floor(n/2)*floor((n-1)/2)*floor((n-2)/2)
 * Range:     u8 n <= 14, u16 n <= 102, u32 n <= 3763, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the product contains a factor 0, so the value is 0. */
    if (n < 3)
        return 0;

    seq_t a = (seq_t)(n / 2);          /* floor(n/2)   */
    seq_t b = (seq_t)((n - 1) / 2);    /* floor((n-1)/2) */
    seq_t c = (seq_t)((n - 2) / 2);    /* floor((n-2)/2) */

    seq_t prod = a;
    prod *= b;
    prod *= c;
    prod /= 2;                         /* divide by 2 */

    return prod;
}
