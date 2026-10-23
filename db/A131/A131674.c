/*
 * A131674  Size of the largest BDD of symmetric Boolean functions of n
 *          variables when the sink nodes are not counted.
 * https://oeis.org/A131674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} min(k, 2^(n+2-k)-2)
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t t = n + 2 - k;          /* exponent of 2 */
        seq_t term;
        if (t <= 63u)                  /* 2^t fits in 64‑bit range */
        {
            seq_t val = pow2(t);      /* 2^t as seq_t */
            val -= 2;                 /* 2^t - 2 */
            if (val <= (seq_t)k)
                term = val;           /* choose the smaller one */
            else
                term = (seq_t)k;
        }
        else
        {
            term = (seq_t)k;           /* power is far larger than k */
        }
        sum += term;
    }
    return sum;
}
