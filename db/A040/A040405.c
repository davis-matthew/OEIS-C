/*
 * A040405  Continued fraction for sqrt(426).
 * https://oeis.org/A040405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of continued fraction terms of sqrt(426) */
/* a(0) = floor(sqrt(426)) = 20; subsequent terms repeat with period 12:
   1,1,1,3,2,6,2,3,1,1,1,40. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 20;

    static seq_t period[128] = {0};
    static size_t period_len = 0;

    if (period_len == 0)
    {
        const seq_t a0 = 20;      /* floor(sqrt(426)) */
        const seq_t D  = 426;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_next = a0;       /* current a_n */
        size_t i = 0;

        do
        {
            /* compute next m and d */
            m = d * a_next - m;
            d = (D - m * m) / d;

            /* compute next a_{n+1} */
            a_next = (a0 + m) / d;

            period[i++] = a_next;           /* store the term a_{n+1} */
        } while (a_next != a0 * 2);          /* period ends when a_{n+1} == 2*a0 */

        period_len = i;
    }

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
