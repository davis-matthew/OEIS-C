/*
 * A159644  Numerator of Hermite(n, 6/19).
 * https://oeis.org/A159644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 12; a(n) = 12*a(n-1) - 722*(n-1)*a(n-2) for n
 *            >= 2 This recurrence follows from the Hermite recurrence
 *            H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x) with x = 6/19 and a(n) =
 *            19^n * H_n(6/19)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_prev = 1;              /* a(0)   */
    if (n == 0) return a_prev;

    seq_t a_cur  = 12;             /* a(1)   */
    if (n == 1) return a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 12*a(i-1) - 722*(i-1)*a(i-2) */
        seq_t a_next = 12 * a_cur - ( (seq_t)722 * (i-1) * a_prev );
        a_prev = a_cur;
        a_cur  = a_next;
    }
    return a_cur;
}
