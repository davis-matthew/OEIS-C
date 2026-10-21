/*
 * A226695  Pell equation solutions (32*b(n))^2 - 41*(5*a(n))^2 = -1 with b(n)
 *          = A226694(n), n >= 0.
 * https://oeis.org/A226695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4098*a(n-1) - a(n-2) with a(-1)=1, a(0)=1 This implements
 *            the OEIS sequence A226695
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t a_minus1 = (seq_t)1;    /* a(-1) = 1, needed for the first step   */
    seq_t a_prev   = (seq_t)1;    /* a(0)  = 1                              */

    seq_t a_curr;                 /* will hold a(i) in the loop            */

    for (idx_t i = 1; i <= n; ++i) {
        a_curr = (seq_t)4098 * a_prev - a_minus1;
        a_minus1 = a_prev;
        a_prev   = a_curr;
    }

    return a_prev;                /* a(n) is now in a_prev                 */
}
