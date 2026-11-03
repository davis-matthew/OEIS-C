/*
 * A073842  a(1) = 1; for n>1, a(n) = the smallest positive integer root of n
 *          not included earlier, if such a root exists, otherwise the
 *          smallest power of n not included earlier, subject to a(n)<>n.
 * https://oeis.org/A073842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   If n = m^(2^k) with m not a perfect square, then a(n) =
 *            m^(2^(k+1)) when k is even, a(n) = m^(2^(k-1)) when k is odd.
 *            (With a(1)=1 as the only fixed point.)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

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
    /* The sequence is defined for n >= 1; a(1) = 1. */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    seq_t v = (seq_t)n;   /* working value: n reduced to m */
    idx_t k = 0;          /* number of successive perfect-square reductions */

    /* Reduce n until it is not a perfect square. */
    while (true) {
        seq_t r = floor_sqrt(v);
        seq_t r2 = r * r;       /* exact square test */
        if (r2 == v) {
            v = r;
            ++k;
        } else {
            break;
        }
    }

    seq_t m = v;   /* m is not a perfect square */

    /* Compute the exponent 2^(k+1) if k is even,
       else 2^(k-1).  The shift is safe for k <= 63. */
    idx_t exp;
    if ((k & 1ULL) == 0) {          /* k even */
        exp = ((idx_t)1) << (k + 1);
    } else {                        /* k odd */
        exp = ((idx_t)1) << (k - 1);
    }

    return ipow_idx(m, exp);
}
