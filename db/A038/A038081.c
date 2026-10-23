/*
 * A038081  Number of rooted identity trees of height n. Sets of rank n.
 * https://oeis.org/A038081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^{e_{n-1}} - e_{n-1}, with e_0 = 1 and e_{k+1} = 2^{e_k}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute e_{n-1} iteratively */
    seq_t e = (seq_t)1;        /* e_0 */
    idx_t i;
    for (i = 0; i < n - 1; ++i) {
        /* Ensure e fits into idx_t before calling pow2() */
        if (e > (seq_t)UINT64_MAX) {
            /* Sequence grows too fast; cannot compute further */
            return (seq_t)0;
        }
        e = pow2((idx_t)e);
    }

    /* Now a(n) = 2^e - e */
    if (e > (seq_t)UINT64_MAX) {
        /* power would be astronomically large; return 0 as a safe fallback */
        return (seq_t)0;
    }
    return pow2((idx_t)e) - e;
}
