/*
 * A028200  Expansion of 1/((1-6x)*(1-7x)*(1-8x)*(1-9x)).
 * https://oeis.org/A028200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 335*a(n-2) + 1650*a(n-3) - 3024*a(n-4),
 *            a(0)=1, a(1)=30, a(2)=565, a(3)=8550
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)565;
    if (n == 3) return (seq_t)8550;

    /* Recurrence uses the last four values */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)30;     /* a(n-3) */
    seq_t a2 = (seq_t)565;    /* a(n-2) */
    seq_t a3 = (seq_t)8550;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = 30 * a3
                   - 335 * a2
                   + 1650 * a1
                   - 3024 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
