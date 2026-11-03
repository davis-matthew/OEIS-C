/*
 * A176190  Triangle T(n,m) read by rows: (1 + binomial(n, m)*binomial(n + 1,
 *          m)/(m + 1) )^n, 0<=m<=n.
 * https://oeis.org/A176190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1 + binomial(r,m)*binomial(r+1,m)/(m+1))^r, where r =
 *            floor((sqrt(8*n+1)-1)/2), m = n - r*(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   binomial, ipow_idx, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Determine the row index r */
    idx_t s = isqrt(1 + 8 * n);
    idx_t r = (s - 1) / 2;

    /* Column index m within that row */
    idx_t cumPrev = r * (r + 1) / 2;
    idx_t m = n - cumPrev;

    /* Compute binomial factors */
    seq_t c1 = binomial(r, m);        /* C(r,m)   */
    seq_t c2 = binomial(r + 1, m);    /* C(r+1,m) */

    /* Compute (1 + C(r,m)*C(r+1,m)/(m+1))^r */
    seq_t prod = c1 * c2;
    seq_t div  = prod / (m + 1);      /* exact division */
    seq_t base = 1 + div;
    return ipow_idx(base, r);
}
