/*
 * A140271  Least divisor of n that is > sqrt(n), with a(1) = 1.
 * https://oeis.org/A140271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest divisor of n that is strictly greater than sqrt(n).
   For a perfect square this is n divided by the largest divisor smaller
   than sqrt(n).  a(1) = 1. */
#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    /* floor integer square root using a binary‑search style shift method */
    idx_t root = 0;
    idx_t bit  = (idx_t)1 << 62;           /* largest power of four <= 2^64 */
    while (bit != 0) {
        idx_t nxt = root + bit;
        if (n >= nxt) {
            n -= nxt;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return root;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safe guard – undefined for 0   */
    idx_t s = isqrt_u64(n);               /* floor(sqrt(n))                 */
    for (idx_t d = s; d > 0; --d) {
        if (n % d == 0) {
            if (d * d == n)             /* skip the sqrt itself          */
                continue;
            return (seq_t)(n / d);      /* corresponding larger divisor   */
        }
    }
    return (seq_t)n;                       /* fallback – only for n=1       */
}
