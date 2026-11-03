/*
 * A131922  Triangle read by rows: 2*A002024 - A130296 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A131922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r = floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2; return r+1
 *            if c==0 else 2*r+1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* compute r from the triangular index formula */
    idx_t t   = isqrt(8 * n + 1);
    idx_t r   = (t - 1) / 2;
    idx_t off = r * (r + 1) / 2;
    idx_t c   = n - off;

    if (c == 0)
        return (seq_t)(r + 1);      /* first element of the row */
    else
        return (seq_t)(2 * r + 1);  /* remaining elements of the row */
}
