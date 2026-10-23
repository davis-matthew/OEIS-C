/*
 * A106246  Number triangle T(n,k)=C(n,k)C(2,n-k).
 * https://oeis.org/A106246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r,k) * C(2,r-k) where r = row_of(n) (the largest r with
 *            r(r+1)/2 ≤ n) and k = n - r(r+1)/2
 * Range:     u8 n <= 321, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   row_of, small_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/row_of.h"
#include "../utils/small_binom.h"

seq_t a(idx_t n)
{
    idx_t r = row_of(n);                                 /* row index */
    idx_t start = (r * (r + 1)) / 2;                     /* starting global index of this row */
    idx_t k = n - start;                                 /* column index within the row */
    idx_t r_minus_k = r - k;

    if (r_minus_k > 2) return 0;                         /* zero outside the non‑zero band */

    seq_t c1 = small_binom(r, k);                        /* C(r,k) */
    seq_t c2;                                            /* C(2,r-k) */
    switch (r_minus_k) {
        case 0: c2 = 1; break;
        case 1: c2 = 2; break;
        case 2: c2 = 1; break;
        default: return 0;  /* safety, should never happen */
    }
    return c1 * c2;
}
