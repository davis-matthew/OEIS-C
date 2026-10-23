/*
 * A021951  Decimal expansion of 1/947.
 * https://oeis.org/A021951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/947 (0‑based)
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rem = 1;          /* initial remainder after dividing 1 by 947 */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t digit = (rem * 10) / 947;  /* floor(10*rem/947) gives the next digit */
        if (i == n)
            return digit;
        rem = (rem * 10) % 947;         /* update remainder for next digit */
    }
    /* The loop always returns, but keep a return to satisfy the compiler */
    return 0;
}
