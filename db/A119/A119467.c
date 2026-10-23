/*
 * A119467  A masked Pascal triangle.
 * https://oeis.org/A119467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, look
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2,
 *            T(r,k) = C(r,k) if (r-k) even, else 0
 * Range:     u8 n <= 39, u16 n <= 198, u32 n <= 646, u64 n <= 2377,
 *            u128 n <= 8000, big any n
 * Helpers:   binomial_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_pos.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (1) {
        __int128 tri_next = (__int128)(r + 1) * (r + 2) / 2;
        if (tri_next > n) break;
        ++r;
    }

    /* Starting index of this row */
    idx_t start = (idx_t)(( (__int128)r * (r + 1) ) / 2);
    idx_t k = n - start;          /* column index */

    /* Entry is non‑zero only when (r - k) is even */
    if (((r - k) & 1) == 0) {
        return binomial_pos(r, k);
    } else {
        return (seq_t)0;
    }
}
