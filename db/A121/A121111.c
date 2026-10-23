/*
 * A121111  Trajectory of 277777788888899 under "x -> product of digits of x"
 *          map.
 * https://oeis.org/A121111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A121111: Trajectory of 277777788888899 under
   the map x → product of digits of x.
   a(1) = 277777788888899
   a(n) = digit_product(a(n-1)) for n > 1,
   returning 0 thereafter. 
*/

#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined below offset, return 0 */
        return (seq_t)0;

    /* starting value of the trajectory */
    seq_t x = (seq_t)277777788888899ULL;

    /* first term already known */
    if (n == 1) return x;

    idx_t step = 1;                /* currently at a(step) */
    while (step < n && x != 0ULL) {
        x = digit_product(x);
        ++step;
    }
    return x;
}
