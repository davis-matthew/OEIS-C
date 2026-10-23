/*
 * A132708  Period 6: repeat [4, 2, 1, -4, -2, -1].
 * https://oeis.org/A132708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132708
 * Period 6: repeat [4, 2, 1, -4, -2, -1]
 * a(0)=4, a(1)=2, a(2)=1, a(3)=-4, a(4)=-2, a(5)=-1
 * Recurrence: a(n)=a(n-6)   (for n ≥ 6)
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
        /* The pattern repeats every 6 indices, so we can take the remainder
         * of n modulo 6 and index into a small static table. */
        static const seq_t pattern[6] = {
                (seq_t) 4,
                (seq_t) 2,
                (seq_t) 1,
                (seq_t)-4,
                (seq_t)-2,
                (seq_t)-1
        };
        return pattern[(size_t)(n % 6)];
}
