/*
 * A213564  Rectangular array: (row n) = b**c, where b(h) = h*(h+1)/2, c(h) =
 *          (n-1+h)^2, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where (r,c) are obtained by mapping n onto the
 *            falling antidiagonals of the array defined by b(h)=h*(h+1)/2 and
 *            c(h)=(r-1+h)^2 via convolution: T(r,c)=∑ₕ₌₁^c b(h)*c(c+1-h)
 * Range:     u8 n <= 15, u16 n <= 190, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline idx_t triangular(idx_t s) {
    return (idx_t)((__int128)s * (s + 1) / 2);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset is 1, but guard */

    /* find diagonal s such that triangular(s-1) < n ≤ triangular(s) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;
    idx_t tri_prev = triangular(s - 1);
    idx_t offset = n - tri_prev;   /* 1‑based position on the diagonal */
    idx_t r = offset;              /* row index (original n in definition) */
    idx_t c = s - offset + 1;       /* column index */

    seq_t sum = 0;
    for (idx_t h = 1; h <= c; ++h) {
        idx_t tindex = r - 1 + (c + 1 - h);   /* position in c sequence */
        seq_t bh = (seq_t)h * ((seq_t)h + 1) / 2;
        seq_t ch = (seq_t)tindex * (seq_t)tindex;
        sum += bh * ch;
    }
    return sum;
}
