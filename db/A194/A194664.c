/*
 * A194664  Triangular array: T(n,k)=[<n*r>+<k*r>], where [ ] = floor, < > =
 *          fractional part, and r = sqrt(2).
 * https://oeis.org/A194664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the sum of the fractional parts of (row+1)*√2 and
 *            (col+1)*√2 is at least 1, 0 otherwise. row and col are the
 *            0‑based coordinates of the triangular array indexed by n with
 *            standard row‑major ordering
 * Range:     u8 n <= 8684, u16 n <= 8684, u32 n <= 8684, u64 n <= 8684,
 *            u128 n <= 8684, big any n
 * Helpers:   floor_sqrt2_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the convergent 239/169 of √2 to compute floor(k*√2)
   exactly for all k up to the limits required by the problem. */
#include <stdint.h>
#include "../utils/floor_sqrt2_val.h"

static inline void linear_to_rc(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    idx_t start = 0;                     /* index of the first entry in row r */
    while (start + r + 1 < n) {
        start += r + 1;
        ++r;
    }
    *row = r;
    *col = n - start - 1;
}

seq_t a(idx_t n)
{
    idx_t r, c;
    linear_to_rc(n, &r, &c);

    seq_t fr = floor_sqrt2_val(r + 1);    /* floor((r+1)*√2) */
    seq_t fc = floor_sqrt2_val(c + 1);    /* floor((c+1)*√2) */
    seq_t fs = floor_sqrt2_val(r + c + 2);/* floor((r+c+2)*√2) */

    return fs - fr - fc;                  /* 0 or 1 */
}
