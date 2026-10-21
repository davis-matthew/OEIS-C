/*
 * A097285  Contains exactly once every pair (i,j) of distinct positive
 *          integers.
 * https://oeis.org/A097285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2 let m be the smallest integer with (m−1)*m ≥ n.
 *            Let p = n − (m−1)*(m−1) − 1 (0‑based position inside the segment
 *            for m). If p is even, a(n)=p/2+1; otherwise a(n)=m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the concatenation of segments: 1 2, 1 3 2 3, 1 4 2 4 3 4, …   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_uint128.h"

seq_t a(idx_t n)
{
    /* Find m such that (m-1)*m >= n.  */
    unsigned __int128 val = (unsigned __int128)4 * n + 1; /* 1 + 4n   */
    uint64_t sqrtv     = isqrt_uint128(val);
    uint64_t m         = (sqrtv + 1) / 2;               /* floor */
    while ((unsigned __int128)(m - 1) * m < n)
        m++;                                            /* safety for floor case   */

    /* Position inside the segment for m.   */
    unsigned __int128 sprev = (unsigned __int128)(m - 2) * (m - 1); /* S(m-1)   */
    idx_t        p = n - (idx_t)sprev - 1;                /* 0‑based  */

    if ((p & 1) == 0) {                                 /* even position   */
        idx_t validx = (p >> 1) + 1;
        return (seq_t)validx;
    } else {                                            /* odd position   */
        return (seq_t)m;
    }
}
