/*
 * A152832  a(0) = -2; a(n) = n - a(n-1) for n > 0.
 * https://oeis.org/A152832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - a(n-1), with a(0) = -2
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Even index:  a(n) = n/2 - 2
        Odd  index:  a(n) = (n + 5)/2   */
    if ((n & 1) == 0) {                 /* even n */
        return ((seq_t)n >> 1) - 2;     /* n/2 - 2 */
    } else {                            /* odd n */
        return ((((seq_t)n) + 5) >> 1); /* (n+5)/2 */
    }
}
