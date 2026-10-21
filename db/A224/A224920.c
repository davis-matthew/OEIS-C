/*
 * A224920  Fifth powers expressed in base 3.
 * https://oeis.org/A224920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal number obtained by writing n^5 in base 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle the offset (a(1) is defined) */
    if (n == 0)
        return (seq_t)0;

    /* compute n^5 using only integer arithmetic */
    seq_t power = (seq_t)n;          /* n^1 */
    power *= power;                  /* n^2 */
    power *= (seq_t)n;                /* n^3 */
    power *= (seq_t)n;                /* n^4 */
    power *= (seq_t)n;                /* n^5 */

    /* transform the base‑3 representation of power into a decimal number
       with the same digits (0,1,2). */
    seq_t result = (seq_t)0;
    seq_t multiplier = (seq_t)1;      /* 10^0 */

    while (power > (seq_t)0) {
        seq_t digit = power % (seq_t)3;   /* lowest base‑3 digit (0,1,2) */
        result += digit * multiplier;
        power /= (seq_t)3;
        if (power > (seq_t)0)            /* avoid overflow on the last step */
            multiplier *= (seq_t)10;
    }

    return result;
}
