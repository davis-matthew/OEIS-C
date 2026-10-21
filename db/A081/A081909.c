/*
 * A081909  a(n) = 3^n(n^2 - n + 18)/18.
 * https://oeis.org/A081909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n*(n^2 - n + 18)/18
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Small indices are handled explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* For n >= 2 the expression simplifies to */
    /* a(n) = 3^(n-2) * ((n^2 - n + 18) / 2).      */
    seq_t pow_part = pow3(n - 2);                 /* 3^(n-2) */

    seq_t nn   = (seq_t)n * (seq_t)n;             /* n^2          */
    seq_t term = nn - (seq_t)n + (seq_t)18;       /* n^2 - n + 18 */
    seq_t term_even = term / (seq_t)2;            /* divide by 2   */

    return pow_part * term_even;
}
