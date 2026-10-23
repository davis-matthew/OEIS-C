/*
 * A027973  a(n) = T(n,n) + T(n,n+1) + ... + T(n,2n), T given by A027960.
 * https://oeis.org/A027973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+2) - Lucas(n+2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas numbers L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2) */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute target index of Lucas */
    idx_t target = n + 2;

    /* 2^(n+2) */
    seq_t pow_val = pow2(target);

    /* Compute Lucas(target) */
    seq_t l_prevprev = (seq_t)2;   /* L(0) */
    seq_t l_prev     = (seq_t)1;   /* L(1) */
    seq_t l_curr = 0;

    if (target == 0) {
        return pow_val - l_prevprev;
    } else if (target == 1) {
        return pow_val - l_prev;
    }

    for (idx_t i = 2; i <= target; ++i) {
        l_curr = l_prev + l_prevprev;
        l_prevprev = l_prev;
        l_prev = l_curr;
    }

    /* a(n) = 2^(n+2) - Lucas(n+2) */
    return pow_val - l_curr;
}
