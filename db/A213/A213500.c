/*
 * A213500  Rectangular array T(n,k): (row n) = b**c, where b(h) = h, c(h) = h
 *          + n - 1, n >= 1, h >= 1, and ** = convolution.
 * https://oeis.org/A213500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j) where T(r,c) = c(c+1)(c+3r‑1)/6. The array is read
 *            by southwest‑falling antidiagonals: for the n‑th term (n ≥ 1)
 *            find the diagonal s with (s+1)(s+2)/2 ≥ n. Let prev = s(s+1)/2,
 *            offset = n‑prev. Then the term equals T(i=offset, j=s+2‑offset)
 * Range:     u8 n <= 55, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find diagonal s and total elements up to it: (s+1)(s+2)/2 */
    idx_t s   = 0;
    idx_t tot = 1;            /* (s+1)(s+2)/2 for current s */
    while (tot < n) {
        ++s;
        tot += s + 1;         /* add next diagonal length (s+1) */
    }
    /* previous total elements up to diagonal s‑1 */
    idx_t prev   = tot - (s + 1);   /* = s(s+1)/2 */
    idx_t offset = n - prev;        /* 1‑based position within diagonal */
    idx_t i = offset;               /* row index */
    idx_t j = s + 2 - i;            /* column index */

    /* compute T(i,j) = j*(j+1)*(j+3*i-1)/6 using 64‑bit intermediates
       to avoid overflow when seq_t is as small as uint8_t. */
    unsigned long long jj = (unsigned long long)j;
    unsigned long long ii = (unsigned long long)i;
    unsigned long long val = jj * (jj + 1) * (jj + 3 * ii - 1) / 6ULL;

    return (seq_t)val;
}
