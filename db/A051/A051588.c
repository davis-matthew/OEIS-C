/*
 * A051588  Number of 3 X n binary matrices such that any 2 rows have a common
 *          1.
 * https://oeis.org/A051588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 3*6^n + 3*5^n - 4^n Recurrence used:
 *            a(0)=0,a(1)=1,a(2)=15,a(3)=175
 *            a(n)=23*a(n-1)-194*a(n-2)+712*a(n-3)-960*a(n-4) for n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)175;

    seq_t a0 = (seq_t)0;   /* a(i-4) */
    seq_t a1 = (seq_t)1;   /* a(i-3) */
    seq_t a2 = (seq_t)15;  /* a(i-2) */
    seq_t a3 = (seq_t)175; /* a(i-1) */
    seq_t an = a3;

    for (idx_t k = 4; k <= n; ++k) {
        an = (seq_t)23 * a3
            - (seq_t)194 * a2
            + (seq_t)712 * a1
            - (seq_t)960 * a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
