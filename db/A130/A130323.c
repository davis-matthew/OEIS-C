/*
 * A130323  A007318^(-1) * A130322.
 * https://oeis.org/A130323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = Σ_{m=0}^{d} (-1)^m * C(r, m) * (d-m+1) * 2^{d-m} where r
 *            = ⌊(√(8n+1)-1)/2⌋ (row index) c = n - r*(r+1)/2 (column index) d
 *            = r - c (difference r‑c) The triangle A130322 has entries
 *            B[i][k] = (i-k+1) * 2^{i-k} (i ≥ k) and a(n) is the inverse
 *            binomial transform of that triangle
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;                /* exponential search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t tri_r = r * (r + 1) / 2;
    idx_t c    = n - tri_r;
    idx_t d    = r - c;

    /* Precompute powers of 2: pow2[t] = 2^t  for 0 <= t <= d */
    seq_t *pow2 = (seq_t *)malloc((d + 1) * sizeof(seq_t));
    if (!pow2)
        return 0;   /* fallback if allocation fails */

    pow2[0] = 1;
    for (idx_t t = 1; t <= d; ++t)
        pow2[t] = pow2[t - 1] * 2;

    seq_t sum = 0;
    seq_t bin = 1;                         /* C(r,0) */
    for (idx_t m = 0; m <= d; ++m) {
        seq_t term = bin * (seq_t)(d - m + 1) * pow2[d - m];
        if (m & 1)
            sum -= term;                   /* (-1)^m */
        else
            sum += term;

        /* Update bin to C(r, m+1) for next iteration */
        if (m < d)
            bin = bin * (seq_t)(r - m) / (seq_t)(m + 1);
    }

    free(pow2);
    return sum;
}
