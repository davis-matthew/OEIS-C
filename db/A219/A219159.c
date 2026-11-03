/*
 * A219159  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement - at the beginning 2 layers counterclockwise, next 2
 *          layers clockwise and so on. T(n,k) read by antidiagonals.
 * https://oeis.org/A219159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n,
 *            and T(i,j) is the entry obtained by filling the table in layers
 *            with m = 2, first two layers counter‑clockwise, next two
 *            clockwise, etc
 * Range:     u8 n <= 135, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 82 terms through n = 9529 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the value in the table at position (i,j).  The table is filled
   in layers with m = 2.  The formula simplifies to:
      if m even (i.e., floor((index‑1)/m) even) => T = A
      else                                            T = B
   where A and B are as described below.  */
static inline seq_t T_entry(idx_t i, idx_t j)
{
    if (j >= i) {
        /* k >= n case */
        idx_t m = (j - 1) / 2;           /* floor((j-1)/2) */
        if ((m & 1) == 0) { /* even m: use A = k^2 - i + 1 */
            return (seq_t)((seq_t)j * (seq_t)j - (seq_t)i + 1);
        } else { /* odd m: use B = (k-1)^2 + i */
            idx_t km1 = j - 1;
            return (seq_t)((seq_t)km1 * (seq_t)km1 + (seq_t)i);
        }
    } else {
        /* n > k case, note the exponent is floor((n-1)/2)+1 */
        idx_t m = ((i - 1) / 2) + 1;
        if ((m & 1) == 0) { /* even m: use A = i^2 - j + 1 */
            return (seq_t)((seq_t)i * (seq_t)i - (seq_t)j + 1);
        } else { /* odd m: use B = (i-1)^2 + j */
            idx_t im1 = i - 1;
            return (seq_t)((seq_t)im1 * (seq_t)im1 + (seq_t)j);
        }
    }
}

/* Integer square root for uint64_t (floor of √n). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t bit = ((idx_t)1 << 62);           /* highest power of four ≤ n */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Return the i,j coordinates (1‑based) of the n‑th antidiagonal element. */
static inline void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* t is the 0‑based diagonal index. */
    idx_t val = 8 * n - 7;
    idx_t t   = (isqrt_u64(val) - 1) / 2;

    *i = n - t * (t + 1) / 2;
    *j = (t * t + 3 * t + 4) / 2 - n;
}

/* Public function required by the exerciser. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, defined as 0 */
    idx_t i, j;
    antidiag_coords(n, &i, &j);
    return T_entry(i, j);
}
