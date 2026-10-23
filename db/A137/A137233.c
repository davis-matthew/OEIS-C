/*
 * A137233  Number of n-digit even numbers.
 * https://oeis.org/A137233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 5 for n = 1, a(n) = 9*10^(n-1)/2 for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 10*a(n-1) for n > 2   */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)5;

    /* base: 10^(n-1)
       multiplied by 9 and halved => 9*10^(n-1)/2 */
    seq_t p = pow10(n - 1U);   /* 10^(n-1) */
    seq_t res = p * (seq_t)9;  /* 9*10^(n-1) */
    res /= (seq_t)2;           /* /2 */
    return res;
}
