/*
 * A235337  Number of integer lattice points inside the square ABCD with side
 *          length n>0 with A(-n*sqrt(2)/2| 0), B(n*sqrt(2)/2| 0), C(0|
 *          n*sqrt(2)/2) and D(-n*sqrt(2)/2| 0).
 * https://oeis.org/A235337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil(n*sqrt(2)/2)^2 - 2*ceil(n*sqrt(2)/2)+1
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute z = ceil(n/√2) = floor(n*√2/2) + 1
       floor(n*√2/2) = floor(floor(n*√2)/2)
       and floor(n*√2) = floor(sqrt(2*n^2))                 */
    seq_t nn   = (seq_t)n * (seq_t)n;          /* n^2                         */
    seq_t t    = nn * (seq_t)2;                /* 2*n^2                       */
    seq_t sq   = floor_sqrt(t);                /* floor(sqrt(2*n^2))          */
    seq_t z    = (sq / (seq_t)2) + (seq_t)1;   /* ceil(n/√2)                  */
    return (seq_t)2 * z * z - (seq_t)2 * z + (seq_t)1;
}
