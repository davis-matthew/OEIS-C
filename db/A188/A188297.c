/*
 * A188297  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=1/sqrt(2), k=2, [
 *          ]=floor.
 * https://oeis.org/A188297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) - floor((n-2)*r) - 1, r = 1/sqrt(2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_div_sqrt2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_div_sqrt2.h"

seq_t a(idx_t n)
{
    /*  n = 1,2 give the value 0 directly.  */
    if (n < 3)
        return 0;

    seq_t f_n  = floor_div_sqrt2(n);
    seq_t f_n2 = floor_div_sqrt2(n - 2);
    seq_t diff = f_n - f_n2;          /* 1 or 2 for n ≥ 3 */
    return diff - 1;                  /* 0 or 1 */
}
