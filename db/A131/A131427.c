/*
 * A131427  A000108(n) preceded by n zeros.
 * https://oeis.org/A131427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = catalan(r) if n = r*(r+3)/2, * where r = (sqrt(9+8n)-3)/2
 *            and r is an integer; * a(n) = 0 otherwise. * This produces the
 *            triangle * 1; * 0,1; * 0,0,2; * 0,0,0,5;
 * Range:     u8 n <= 6, u16 n <= 89, u32 n <= 229, u64 n <= 739,
 *            u128 n <= 2554, big any n
 * Validated: standard, pass, 107 terms through n = 9828 (2026-11-01)
 * Helpers:   catalan, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
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
    /*  D = 9 + 8n  */
    seq_t D = (seq_t)9 + (seq_t)8 * (seq_t)n;
    seq_t s = floor_sqrt(D);

    /* Check if D is a perfect square.  */
    if (s * s != D)
        return 0;

    /* (s - 3) must be even and non‑negative. */
    if (s < (seq_t)3)
        return 0;
    seq_t diff = s - (seq_t)3;
    if ((diff & 1) != 0)
        return 0;

    /* r = (s - 3) / 2 */
    idx_t r = (idx_t)(diff >> 1);

    /* Return the r‑th Catalan number.  */
    return catalan(r);
}
