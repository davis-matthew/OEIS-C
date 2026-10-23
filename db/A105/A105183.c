/*
 * A105183  a(n) = 1 + a(n-1)*(a(n-1) + 1), with a(0)=2.
 * https://oeis.org/A105183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = 1 + a(n-1)*(a(n-1) + 1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)2;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = a_val * (a_val + (seq_t)1);  /* a(n-1) * (a(n-1) + 1) */
        a_val = (seq_t)1 + prod;                /* 1 + prod */
    }
    return a_val;
}
