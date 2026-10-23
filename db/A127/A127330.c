/*
 * A127330  Begin with the empty sequence and a starting number s = 0. At step
 *          k (k >= 1) append the k consecutive numbers s to s+k-1 and change
 *          the starting number (for the next step) to 2s+2.
 * https://oeis.org/A127330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 497, u64 n <= 2017,
 *            u128 n <= 8000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127330: a(0)=0; at step k append the k consecutive numbers
   starting from s_k = 2^k - 2; next start s_{k+1} = 2*s_k + 2.
   For any n>=0, let t be the smallest integer with t(t+1)/2 > n.
   Then a(n) = 2^t - 2 + (n - (t-1)*t/2). */
#include <stdint.h>
#include "../utils/pow2.h"

static inline seq_t tri_uint64(uint64_t k)
{
    return (seq_t)k * (k + 1) / 2;
}

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;            /* n fits in uint64_t by definition */

    /* Find t such that Tri(t-1) <= n < Tri(t) */
    uint64_t lo = 0;
    uint64_t hi = 1;
    while (tri_uint64(hi) <= (seq_t)u && hi <= ((uint64_t)-1)/2)
        hi <<= 1;

    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1); /* avoid overflow   */
        if (tri_uint64(mid) <= (seq_t)u)
            lo = mid;
        else
            hi = mid - 1;
    }
    uint64_t step = lo + 1;                          /* t                    */
    uint64_t prev_tri = (uint64_t)tri_uint64(step - 1);
    uint64_t offset = u - prev_tri;                 /* position inside step */

    seq_t val = pow2(step) - (seq_t)2;
    val += (seq_t)offset;
    return val;
}
