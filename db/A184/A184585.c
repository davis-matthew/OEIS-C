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
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = floor( (5*(3n+1) + floor((3n+1)*sqrt(5))) / 12 ) */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
