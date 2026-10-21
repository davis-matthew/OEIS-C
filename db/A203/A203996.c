/*
 * A203996  Symmetric matrix based on f(i,j)=min{i(j+1),j(i+1)}, by
 *          antidiagonals.
 * https://oeis.org/A203996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min{ i*(j+1), j*(i+1) } where (i,j) are the coordinates
 *            on the antidiagonal of index n (enumeration by increasing i+j)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt_idx_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n < 1, but safe guard */

    /* Find the diagonal s such that C(s-1) < n <= C(s), where
       C(k) = k*(k-1)/2.  Use a lower bound derived from the quadratic. */
    idx_t disc      = 1 + 8 * n;
    idx_t sqrt_disc = floor_sqrt_idx_t(disc);
    idx_t s = (1 + sqrt_disc) / 2;                /* lower bound of s */
    while (s * (s - 1) / 2 < n) s++;              /* adjust upwards if needed */

    idx_t prev_cum = (s - 1) * (s - 2) / 2;        /* C(s-1) */
    idx_t offset   = n - prev_cum - 1;             /* zero‑based offset on the diagonal */

    idx_t i = offset + 1;
    idx_t j = s - i;

    seq_t val1 = (seq_t)i * ((seq_t)j + 1);
    seq_t val2 = (seq_t)j * ((seq_t)i + 1);
    return val1 < val2 ? val1 : val2;
}
