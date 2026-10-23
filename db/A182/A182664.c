/*
 * A182664  a(n) = A088828(n) + A157502(n).
 * https://oeis.org/A182664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + 2*sum_{k=1}^{n-1} T(k), where T(k)=1 iff k is
 *            triangular, else 0. Equivalently a(n) = 1 + 4*n +
 *            2*floor((sqrt(8*n-7)-1)/2)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Integer square root for idx_t (unsigned 64‑bit) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (n >> 1) + 1;            /* upper bound for sqrt(n)   */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n)
            return mid;
        if (sq < (unsigned __int128)n)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* number of triangular numbers less than n */
    idx_t t = n * 8 - 7;                 /* 8*n-7 (fits in idx_t for typical n) */
    idx_t sq = isqrt_u64(t);             /* floor(sqrt(8*n-7)) */
    idx_t cnt = (sq > 0) ? (sq - 1) / 2 : 0;

    seq_t result = 1 + (seq_t)4 * (seq_t)n + (seq_t)2 * (seq_t)cnt;
    return result;
}
