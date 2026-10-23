/*
 * A040614  Continued fraction for sqrt(640).
 * https://oeis.org/A040614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040614: continued fraction terms of sqrt(640)
   a(0) = 25
   For n >= 1: a(n) = period[(n-1) % 12],
   where period = {3,2,1,4,1,11,1,4,1,2,3,50}.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;

    static const seq_t period[12] = {
        (seq_t)3,  (seq_t)2,  (seq_t)1,  (seq_t)4,
        (seq_t)1,  (seq_t)11, (seq_t)1,  (seq_t)4,
        (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)50
    };

    return period[(size_t)((n - 1) % 12)];
}
