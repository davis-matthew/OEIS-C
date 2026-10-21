/*
 * A085440  a(n) = Sum_{i=1..n} binomial(i+1,2)^5.
 * https://oeis.org/A085440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} binomial(i+1,2)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 95, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t t  = ii * (ii + 1) / 2;        /* binomial(i+1,2) */
        seq_t t2 = t * t;                    /* t^2           */
        seq_t t3 = t2 * t;                   /* t^3           */
        seq_t t4 = t3 * t;                   /* t^4           */
        seq_t t5 = t4 * t;                   /* t^5           */
        total += t5;
    }
    return total;
}
