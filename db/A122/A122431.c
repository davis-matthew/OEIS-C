/*
 * A122431  Riordan array ((1+x)^3,x).
 * https://oeis.org/A122431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth element of the row‑major flattening of the
 *            triangle T(n,k) = C(3, n-k), where 0 ≤ k ≤ n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine row index r and offset within the row. */
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 <= n) {
        r++;
    }
    r--; /* now r satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */

    seq_t row_start = (seq_t)r * (r + 1) / 2;
    seq_t offset = n - row_start;          /* 0 ≤ offset ≤ r */
    idx_t k = (idx_t)offset;               /* column index within the row */
    idx_t m = r - k;                       /* exponent in C(3, m) */

    /* Compute C(3, m). */
    seq_t val;
    switch (m) {
        case 0:  val = 1; break;
        case 1:  val = 3; break;
        case 2:  val = 3; break;
        case 3:  val = 1; break;
        default: val = 0;
    }
    return val;
}
