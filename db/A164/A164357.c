/*
 * A164357  Expansion of (1 - x^2)^5 / ((1 - x)^4 * (1 - x^6)) in powers of x.
 * https://oeis.org/A164357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in (1 - x^2)^5 / ((1 - x)^4 * (1
 *            - x^6)) Sequence: 1,4,5,0,-5,-4,0,4,5,0,-5,-4, ... (period 6 for
 *            n>0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 1;

    /* Pattern repeats every 6 for n > 0 */
    static const seq_t pat[6] =
    {
        0,          /* n % 6 == 0  -> a(n) = 0 for n > 0 */
        4,          /* n % 6 == 1  -> a(n) = 4 */
        5,          /* n % 6 == 2  -> a(n) = 5 */
        0,          /* n % 6 == 3  -> a(n) = 0 */
        (seq_t)-5,  /* n % 6 == 4  -> a(n) = -5 */
        (seq_t)-4   /* n % 6 == 5  -> a(n) = -4 */
    };

    return pat[(int)(n % 6)];
}
