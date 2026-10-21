/*
 * A076678  a(0)=a(1)=1 a(n)=floor(sqrt(a(n-1)))+a(n-2).
 * https://oeis.org/A076678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(a(n-1))) + a(n-2), a(0)=a(1)=1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = floor_sqrt_seq(a1) + a0;
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
