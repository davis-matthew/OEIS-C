/*
 * A131054  Triangle read by rows: A000012 * A119467 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A131054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{m=0}^{⌊(r−c)/2⌋} binom(c+2m , c) where r =
 *            ⌊(√(8n+1)−1)/2⌋ (row index) c = n − r(r+1)/2 (column index)
 *            binom(p,q) is the standard binomial coefficient
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrt(8.0 * (long double)n + 1.0) - 1.0) / 2.0);
    /* adjust in case of floating‑point rounding */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    /* column index */
    idx_t c = n - r * (r + 1) / 2;
    /* number of terms to sum */
    idx_t t = (r - c) / 2;
    /* compute the sum */
    seq_t sum = 0;
    for (idx_t m = 0; m <= t; ++m)
        sum += binom(c + 2 * m, c);
    return sum;
}
