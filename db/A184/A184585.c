/*
 * A184585  a(n) = floor(n*s + c*s), where r = sqrt(5), c = 1/3, s = r/(r-1);
 *          complement of A184584.
 * https://oeis.org/A184585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ((3n+1)*(5 + sqrt(5))) / 12 )
 * Range:     u8 none, u16 n <= 18, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = floor( (5*(3n+1) + floor((3n+1)*sqrt(5))) / 12 ) */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Computes floor( (5 + sqrt(5)) * (3n + 1) / 12 )  */
seq_t a(idx_t n)
{
    /* m = 3*n + 1  */
    seq_t m = (seq_t)3 * (seq_t)n + 1;

    /* t1 = 5 * m  */
    seq_t t1 = (seq_t)5 * m;

    /* t2 = floor( m * sqrt(5) )  */
    /*  m * sqrt(5) = sqrt(5 * m * m)  */
    seq_t prod = (seq_t)5 * m * m;
    seq_t t2 = floor_sqrt(prod);

    /* result = (t1 + t2) / 12  */
    return (t1 + t2) / 12;
}
