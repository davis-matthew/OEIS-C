/*
 * A095827  a(n) is the smallest k such that ((A007953)^k)(9n)=9.
 * https://oeis.org/A095827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal k such that applying the decimal digit‐sum
 *            function k times to 9n yields 9
 * Range:     u8 n <= 28, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 9;          /* starting value 9n          */
    seq_t steps = 0;                 /* number of iterations      */

    while (x >= 10) {                /* still more than one digit   */
        seq_t s = 0;
        seq_t y = x;
        while (y > 0) {              /* sum its decimal digits     */
            s += y % 10;
            y /= 10;
        }
        x = s;                       /* next iterate               */
        steps += 1;
    }
    return steps;                    /* thence x is the digit 9     */
}
