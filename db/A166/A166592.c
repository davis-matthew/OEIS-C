/*
 * A166592  Hankel transform of A166588(n-1).
 * https://oeis.org/A166592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) - a(n-4) with a(0)=0,a(1)=1,a(2)=3,a(3)=2. The
 *            sequence is periodic with period 12:
 *            0,1,3,2,3,1,0,-1,-3,-2,-3,-1
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t seq[12] = {
        0,  1,  3,  2,  3,  1,  0, -1, -3, -2, -3, -1
    };
    return seq[(size_t)(n % 12ULL)];
}
