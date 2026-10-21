/*
 * A078638  a(n) = rad(n*(n+1)*(n+2)*(n+3)).
 * https://oeis.org/A078638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n*(n+1)*(n+2)*(n+3))
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* The product n*(n+1)*(n+2)*(n+3) fits safely in idx_t for all n
       that may be requested during normal tests, and rad accepts an
       idx_t argument. */
    idx_t prod = (idx_t)n * (n + 1) * (n + 2) * (n + 3);
    return rad(prod);
}
