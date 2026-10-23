/*
 * A041828  Numerators of continued fraction convergents to sqrt(435).
 * https://oeis.org/A041828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for numerators of convergents to sqrt(435)
 * a(0) = 20
 * a(1) = 21
 * for n >= 2:
 *     a(n) = q(n) * a(n-1) + a(n-2)
 * where q(0) = 20 and for  n > 0  the sequence of q(n) repeats
 * 1, 5, 1, 40, 1, 5, 1, 40, ...
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)20;          /* a(0) */
    seq_t h_minus2 = 0;    /* h_{-2} */
    seq_t h_minus1 = 1;    /* h_{-1} */
    seq_t h_curr   = 0;
    static const seq_t period[4] = {
        (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)40       /* 1,5,1,40 repeats */
    };

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t q;
        if (i == 0)
            q = (seq_t)20;                      /* q(0) = 20 */
        else
            q = period[(i - 1) & 3];             /* period length 4 */

        h_curr = q * h_minus1 + h_minus2;       /* recurrence */
        h_minus2 = h_minus1;
        h_minus1 = h_curr;
    }
    return h_curr;      /* h_n = a(n) */
}
