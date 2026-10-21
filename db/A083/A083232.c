/*
 * A083232  a(n) = (3*7^n+(-1)^n)/4.
 * https://oeis.org/A083232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*7^n + (-1)^n)/4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the first term directly. */
    if (n == 0)
        return (seq_t)1;

    seq_t value = (seq_t)1;  /* a(0) */
    bool sign = true;        /* (-1)^0 = 1 */

    for (idx_t i = 0; i < n; ++i)
    {
        /* Compute a(i+1) = 7*a(i) - 2*(-1)^i. */
        seq_t next = value * (seq_t)7;
        if (sign)          /* i even: subtract 2 */
            next -= (seq_t)2;
        else               /* i odd : add 2  */
            next += (seq_t)2;

        value = next;
        sign = !sign;      /* flip the sign for the next iteration */
    }

    return value;
}
