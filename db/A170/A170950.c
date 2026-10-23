/*
 * A170950  Inverse permutation to A170949.
 * https://oeis.org/A170950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ceil_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse permutation to A170949.  For each integer n ≥ 1:
   1. Find the row r = ceil(sqrt(n)).
   2. The row starts at index start = (r‑1)² + 1 and has length 2r‑1.
   3. Let pos = n – start (0‑based position in the row),
      mid = r‑1 (offset of the centre element within the row),
      k   = (pos + 1) / 2.
   4. If pos == 0          → offs = mid;
      else if pos      is odd → offs = mid – k;
      else                      offs = mid + k.
   5. a(n) = start + offs.
 */
#include <stdint.h>                 /* for the supplied types */
#include "../utils/ceil_sqrt_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* safety for offset 1 */
        return 0;

    idx_t r   = ceil_sqrt_idx(n);   /* row number (1‑based) */
    idx_t mid = r - 1;              /* middle offset in the row */
    idx_t start = mid * mid + 1;    /* first index of this row */
    idx_t pos  = n - start;         /* position in the row (0‑based) */

    idx_t k = (pos + 1) / 2;        /* step number after the first element */
    idx_t offs;
    if (pos == 0) {
        offs = mid;
    } else if (pos & 1U) {          /* odd position → left side of the middle */
        offs = mid - k;
    } else {                        /* even position → right side of the middle */
        offs = mid + k;
    }

    seq_t base = (seq_t)start;
    return base + (seq_t)offs;
}
