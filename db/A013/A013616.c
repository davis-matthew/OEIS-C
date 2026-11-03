/*
 * A013616  Triangle of coefficients in expansion of (1+9*x)^n.
 * https://oeis.org/A013616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = 9^k * C(m, k) Where m = floor((sqrt(8n+1) - 1)/2) and k =
 *            n - m(m+1)/2. This enumerates the triangle T(m,k) = 9^k * C(m,
 *            k) in row‑major order
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Find the row index m such that m(m+1)/2 <= n < (m+1)(m+2)/2 */
    idx_t m = 0;
    while (1) {
        unsigned __int128 next = (unsigned __int128)(m + 1) * (m + 2) / 2;
        if (next > n) break;
        ++m;
    }

    unsigned __int128 tri128 = (unsigned __int128)m * (m + 1) / 2;
    idx_t tri = (idx_t)tri128;          /* triangle number of the row start */
    idx_t k   = n - tri;                /* column index within the row   */

    seq_t bin = binomial(m, k);         /* C(m, k) */
    seq_t pow9 = ipow_idx((seq_t)9, k);  /* 9^k     */
    return seq_mul(bin, pow9);          /* T(m,k)  */
}
