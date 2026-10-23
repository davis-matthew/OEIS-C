/*
 * A101496  Expansion of (1-x^2)/(1-x-x^2+x^3+x^4).
 * https://oeis.org/A101496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)-a(n-4); a(0)=a(1)=a(2)=a(3)=1
 * Range:     i8 n <= 20, i16 n <= 41, i32 n <= 80, i64 n <= 161,
 *            i128 n <= 326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 4) /* all initial terms are 1 */
        return (seq_t)1;

    /* maintain a(n-4), a(n-3), a(n-2), a(n-1) in order */
    seq_t a0 = 1; /* a(n-4) */
    seq_t a1 = 1; /* a(n-3) */
    seq_t a2 = 1; /* a(n-2) */
    seq_t a3 = 1; /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = a3 + a2 - a1 - a0; /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
