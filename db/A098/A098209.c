/*
 * A098209  a(n) = A067275(n+1)^2 - 1.
 * https://oeis.org/A098209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (4/9)*(10^n * (1 + 10^n) - 2)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t power = pow10(n);          /* 10^n                        */
    seq_t part  = power * power;     /* 10^(2n)                     */
    part += power;                   /* +10^n                       */
    part -= (seq_t)2;                /* -2                          */
    seq_t res   = part * (seq_t)4;   /* *4                          */
    res /= (seq_t)9;                 /* /9 (exact division)         */
    return res;
}
