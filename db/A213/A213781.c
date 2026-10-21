/*
 * A213781  Rectangular array: (row n) = b**c, where b(h) = 1+[h/2], c(h) =
 *          n-1+h, n>=1, h>=1, [ ] = floor, and ** = convolution.
 * https://oeis.org/A213781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A213781 is obtained by reading the infinite 2‑D array T(i,j)
 *            (i,j≥1) along falling antidiagonals. Row i is the convolution of
 *            the sequences b(h) = 1 + floor(h/2) (1,2,2,3,3,4,4,…) and c(h) =
 *            (i-1)+h i,(i+1),(i+2),… i.e. T(i,j) = ∑_{h=1}^{j} b(h) *
 *            c(j−h+1) which simplifies to T(i,j) = (i+j)·∑_{h=...
 * Range:     u8 n <= 78, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* find diagonal d such that (d-1)*d/2 ≥ n */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n)
        ++d;

    /* start index of this diagonal */
    idx_t start = (d - 2) * (d - 1) / 2 + 1;
    idx_t offset = n - start;           /* 0‑based within diagonal */
    idx_t i = offset + 1;
    idx_t j = d - i;

    /* compute T(i,j) */
    seq_t sum_b = 0;
    seq_t sum_hb = 0;
    for (idx_t h = 1; h <= j; ++h)
    {
        seq_t b = 1 + (seq_t)(h / 2);
        sum_b += b;
        sum_hb += b * (seq_t)h;
    }
    seq_t s = (seq_t)i + (seq_t)j;
    return s * sum_b - sum_hb;
}
