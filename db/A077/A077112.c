/*
 * A077112  a(n)=n^2 times nearest cube to n^2.
 * https://oeis.org/A077112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^2 * nearest_cube(n^2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

/* Return the cube (m³) nearest to x, where x is a seq_t.  */
static inline seq_t nearest_cube(seq_t x)
{
    seq_t c   = floor_cube_root(x);           /* largest m with m³ <= x        */
    seq_t m   = c + 1;                          /* m+1 */
    seq_t cube1 = c * c * c;                   /* c³           */
    seq_t cube2 = m * m * m;                   /* (c+1)³       */
    seq_t diff1 = x - cube1;                   /* x - c³  (>=0)*/
    seq_t diff2 = cube2 - x;                   /* (c+1)³ - x (>=0)*/
    return (diff1 <= diff2) ? cube1 : cube2;   /* nearest cube  */
}

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;           /* n²           */
    seq_t nc   = nearest_cube(nsq);           /* nearest cube to n² */
    return nsq * nc;                          /* n² * nearest cube */
}
