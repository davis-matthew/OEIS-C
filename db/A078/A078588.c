/*
 * A078588  a(n) = 1 if the integer multiple of phi nearest n is greater than
 *          n, otherwise 0, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A078588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + floor(sqrt(5*n*n))) mod 2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn * 5;                 /* 5*n^2  */
    seq_t s = floor_sqrt(temp);                /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */
    seq_t sum = nn + s;
    return sum & 1;                            /* (n + floor(n*sqrt5)) mod 2 */
}
