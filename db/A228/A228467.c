/*
 * A228467  Recurrence a(n) = 2^n*a(n-1) - a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A228467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n·a(n-1) – a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a0 = 0;
    seq_t a1 = 1;
    seq_t power = pow2(2);      /* 2^2 = 4 */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = power * a1 - a0;   /* a(i) = 2^i * a(i-1) - a(i-2) */
        a0 = a1;
        a1 = ai;
        power <<= 1;            /* prepare 2^(i+1) for next step */
    }
    return a1;
}
