/*
 * A188076  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(3), k=5, [ ]=floor.
 * https://oeis.org/A188076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*sqrt(3)] - [5*sqrt(3)] - [n*sqrt(3)-5*sqrt(3)]
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* n as seq_t */
    seq_t three = (seq_t)3;         /* constant 3 as seq_t */

    /* floor(n*sqrt(3)) = floor_sqrt(3 * n^2) */
    seq_t val1 = floor_sqrt( three * nn * nn );

    if (n >= 5) {
        /* n >= 5  ->  use + formula : a = v1 - 8 - v2 */
        seq_t diff = (seq_t)(n - 5);
        seq_t val2 = floor_sqrt( three * diff * diff ); /* floor((n‑5)*sqrt3) */
        seq_t sum  = (seq_t)8 + val2;                    /* 8 + v2          */
        if (val1 < sum) return (seq_t)0;
        return val1 - sum;
    } else {
        /* n < 5  ->  a = v1 - 7 + floor( (5‑n)*sqrt3 ) */
        seq_t m   = (seq_t)(5 - n);
        seq_t temp = floor_sqrt( three * m * m ); /* floor((5-n)*sqrt3) */
        seq_t sum  = val1 + temp;                 /* v1 + temp        */
        seq_t seven = (seq_t)7;
        if (sum < seven) return (seq_t)0;
        return sum - seven;
    }
}
