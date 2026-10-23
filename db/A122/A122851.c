/*
 * A122851  Number triangle T(n,k) = C(k,n-k)*(n-k)!.
 * https://oeis.org/A122851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,j) where * r = ⌊(√(8n+1) – 1)/2⌋, * j = n – r(r+1)/2,
 *            * T(r,j) = 0 if 2j < r, * otherwise T(r,j) = ∏_{t=2j‑r+1}^{j} t
 * Range:     u8 n <= 60, u16 n <= 144, u32 n <= 337, u64 n <= 840,
 *            u128 n <= 2245, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (1) {
        idx_t nxt = (r + 1) * (r + 2) / 2;
        if (n < nxt) break;
        r++;
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t j = n - offset;

    if ((j << 1) < r)   /* 2*j < r  -> value is zero */
        return (seq_t)0;

    idx_t start = (j << 1) - r + 1; /* 2j - r + 1 */
    seq_t val = 1;
    for (idx_t x = start; x <= j; ++x)
        val *= (seq_t)x;

    return val;
}
