/*
 * A133293  First differences of A133292.
 * https://oeis.org/A133293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is periodic with period 9:
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = {0,1,2,3,-5,5,-3,-2,-1}[n mod 9] */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic table of the nine distinct values */
    static const seq_t table[9] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)2,
        (seq_t)3,
        (seq_t)-5,
        (seq_t)5,
        (seq_t)-3,
        (seq_t)-2,
        (seq_t)-1
    };
    /* Use modulo 9 to index into the period */
    return table[(size_t)(n % 9)];
}
