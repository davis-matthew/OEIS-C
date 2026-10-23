/*
 * A102560  Expansion of (1-x^3)/(1-x^4).
 * https://oeis.org/A102560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n%4==0, 0 if n%4==1 or 2, -1 if n%4==3
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */

seq_t a(idx_t n)
{
    /* Compute n modulo 4 using a bitwise AND (fast for uint64_t). */
    idx_t r = n & 3;
    switch (r)
    {
        case 0: return 1;    /* 1, 0, 0, -1 repeats every 4 terms */
        case 1: return 0;
        case 2: return 0;
        case 3: return -1;
    }
    return 0;  /* unreachable, keeps the compiler happy */
}
