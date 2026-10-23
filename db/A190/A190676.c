/*
 * A190676  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),3,0) and [ ]=floor.
 * https://oeis.org/A190676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [3*n*sqrt(3)] - 3*[n*sqrt(3)]
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute floor( sqrt(m * n^2) ) for integer m and n */
static inline seq_t floor_sqrt_mult(idx_t n, uint64_t m)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    return floor_sqrt((seq_t)m * nn);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1; 0 is undefined */
    seq_t t1 = floor_sqrt_mult(n, 27); /* floor(3*n*sqrt(3))  */
    seq_t t2 = floor_sqrt_mult(n, 3);  /* floor(n*sqrt(3))    */
    return t1 - (seq_t)3 * t2;
}
