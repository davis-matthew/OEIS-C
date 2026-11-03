/*
 * A064912  Period of continued fraction for sqrt(17)*n.
 * https://oeis.org/A064912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the period length of the continued fraction of sqrt(17)
 *            * n, i.e. of sqrt(17*n^2). The period equals the number of steps
 *            before the first occurrence of 2*a0 in the standard square‑root
 *            continued‑fraction algorithm
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* n is never 0 for honest queries, but we guard against it. */
    if (n == 0) return 0;

    seq_t N   = (seq_t)n;          /* n as a seq_t */
    seq_t D   = N * N * (seq_t)17; /* D = 17*n^2 */
    seq_t a0  = floor_sqrt(D);     /* integer part of sqrt(D) */
    seq_t twoA0 = a0 + a0;         /* 2*a0, termination condition */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;
    seq_t count = 0;

    for (;;) {
        /* compute next triple (m,d,a) */
        m = d * a_cur - m;
        seq_t m_sq = m * m;
        seq_t num  = D - m_sq;
        d = num / d;
        a_cur = (a0 + m) / d;

        ++count;
        if (a_cur == twoA0) break;
    }
    return count;
}
