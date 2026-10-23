/*
 * A139413  Triangle read by rows: row n gives the numbers A010888(n*k) for k
 *          = 1..n.
 * https://oeis.org/A139413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, base
 * Formula:   a(n) = digital_root( r * k ), where r = ceil( (sqrt(8n+1)-1)/2 )
 *            and k = n - r(r-1)/2 (x%9)+(x>0)*(x%9==0)*8 = digital_root(x)
 *            for x>0
 * Range:     u8 n <= 3469, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Determine the row index r (≥1) such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t t   = 8 * n + 1;
    idx_t sr  = _floor_isqrt_u64(t);
    idx_t r   = (sr - 1) / 2;     /* floor of (sqrt(8n+1)-1)/2 */
    if (r * (r + 1) / 2 < n)
        r++;                     /* adjust to the ceil value */

    /* Column index k within the row */
    idx_t k = n - r * (r - 1) / 2;

    /* Product r*k */
    seq_t prod = (seq_t)r * (seq_t)k;

    /* Digital root of prod (1..9, prod>0) */
    seq_t dr = (prod - 1) % 9 + 1;
    return dr;
}
