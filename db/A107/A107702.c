/*
 * A107702  Triangle related to guillotine partitions of a k-dimensional box
 *          by n hyperplanes.
 * https://oeis.org/A107702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row such that * r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, * k = n - r(r+1)/2, * T(r,k) = Σ_{j=0..k}
 *            binom(k+j,2j) · (r-k)^j · C(j), * C(j) = binom(2j, j)/(j+1)
 *            (Catalan number). * This enumerates the triangle row‑wise
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 100, u64 n <= 246,
 *            u128 n <= 655, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find row r and column k (0‑based) for index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k). */
    seq_t sum = 0;
    seq_t base = (seq_t)(r - k);  /* (r-k) is non‑negative */
    seq_t pow  = 1;               /* (r-k)^j, start with j=0 */

    for (idx_t j = 0; j <= k; ++j) {
        if (j > 0)
            pow = pow * base;      /* next power of (r-k) */

        seq_t bin     = binom(k + j, 2 * j);
        seq_t catalan = binom(2 * j, j) / (seq_t)(j + 1);
        seq_t term    = bin * pow * catalan;
        sum += term;
    }
    return sum;
}
