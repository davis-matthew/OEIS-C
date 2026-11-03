/*
 * A127859  a(n) = r(A127858(n)), where r if the cyclic replacement map of the
 *          digits d of n in base 12 defined by d->d+1 if d<11 and d->0 if
 *          d=11.
 * https://oeis.org/A127859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*(12^n - 1)/11 + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* compute 12^n */
    seq_t p = ipow_idx(12, n);
    /* (12^n - 1) */
    seq_t t = p - 1;
    /* multiply by 6 */
    t = t * 6;
    /* divide by 11 (exact) */
    t = t / 11;
    /* add 1 */
    return t + 1;
}
