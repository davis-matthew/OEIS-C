/*
 * A213822  Rectangular array: (row n) = b**c, where b(h) = 3*h-1, c(h) =
 *          3*n-4+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) reads the rectangular array by falling antidiagonals, where
 *            row r contains the convolution of two sequences: b(h) = 3*h – 1
 *            c(h) = 3*r – 4 + 3*h Then a(n) = T(i,j) = Σ_{h=1}^{j}
 *            b(h)*c(j+1-h), with (i,j) the coordinates found from the
 *            antidiagonal index n
 * Range:     u8 n <= 15, u16 n <= 253, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* Find the diagonal s such that total elements up to s is >= n. */
    uint64_t val = (uint64_t)n;
    uint64_t s = 2;
    while ((s * (s - 1) / 2) < val) ++s;   /* s can be at most ~√(2n) */

    /* Number of entries before diagonal s */
    uint64_t total_before = (s - 2) * (s - 1) / 2;

    /* 0‑based offset within the diagonal */
    uint64_t offset = val - total_before - 1;

    /* Coordinates: row i = offset+1, column j = s - i */
    uint64_t row = offset + 1;
    uint64_t col = s - row;

    /* Compute the convolution sum for T(row, col) */
    seq_t result = 0;
    for (uint64_t h = 1; h <= col; ++h)
    {
        seq_t b_val = 3 * (seq_t)h - 1;                 /* b(h)   */
        uint64_t l = col + 1 - h;                      /* index for c */
        seq_t c_val = 3 * (seq_t)row - 4 + 3 * (seq_t)l;/* c(l)   */
        result += b_val * c_val;
    }

    return result;
}
