/*
 * A231151  Least integer k such that n+1 + ... + n+k > 1 + ... + n.
 * https://oeis.org/A231151
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least integer k such that n+1 + … + n+k > 1 + … + n.
 *            Equivalent to solving the quadratic inequality k^2 + (2n+1)k -
 *            n(n+1) > 0. The positive root is r = ( - (2n+1) + sqrt(8n^2 + 8n
 *            + 1) ) / 2 . The desired k equals floor(r)+1; this can be
 *            computed with a single integer square‑root and...
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 995 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* ensure n is treated as seq_t first */
    seq_t n_seq = (seq_t)n;

    /* compute S = 8*n^2 + 8*n + 1 */
    seq_t S = (seq_t)8 * n_seq * n_seq + (seq_t)8 * n_seq + (seq_t)1;

    /* integer floor of sqrt(S) */
    seq_t sqrtS = floor_sqrt(S);

    /* compute g = sqrtS - (2n + 1) */
    seq_t two_n_plus_one = (seq_t)2 * n_seq + (seq_t)1;
    seq_t g = sqrtS - two_n_plus_one;

    /* k = ceil(g/2) if g is odd, else g/2 + 1 */
    seq_t k;
    if ((g & (seq_t)1))          /* g is odd */
        k = (g + (seq_t)1) / (seq_t)2;
    else                          /* g is even */
        k = (g + (seq_t)2) / (seq_t)2;

    return k;
}
