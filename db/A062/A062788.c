/*
 * A062788  Squares of the form 5n*(n-1)+1.
 * https://oeis.org/A062788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=361, a(3)=116281 a(n)=323*a(n-1)-323*a(n-2)+a(n-3)
 *            for n>3 (OEIS A062788)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) is defined in the sequence */
    if (n == 0)
        return 0;          /* behaviour below offset is irrelevant */

    /* handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)361;
    if (n == 3) return (seq_t)116281;

    /* iterate using the recurrence, keeping only the last three values */
    seq_t a_nm3 = (seq_t)1;          /* a(n-3) for the current step      */
    seq_t a_nm2 = (seq_t)361;        /* a(n-2)                           */
    seq_t a_nm1 = (seq_t)116281;     /* a(n-1)                           */
    seq_t a_n   = 0;                 /* a(n) for current step            */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)(( ( (seq_t)323 * a_nm1 ) - ( (seq_t)323 * a_nm2 ) + a_nm3 ));
        /* roll the window: a(n-3)=a(n-2), a(n-2)=a(n-1), a(n-1)=a(n) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
