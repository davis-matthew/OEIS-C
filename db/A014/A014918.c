/*
 * A014918  a(1)=1, a(n) = n*6^(n-1) + a(n-1).
 * https://oeis.org/A014918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n*(5*n-1)+1)/25 Recurrence: a(1)=1, a(2)=13, a(3)=121,
 *            a(n)=13*a(n-1)-48*a(n-2)+36*a(n-3) for n>3 Works for all n≥1 and
 *            for any seq_t that can hold the result
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset base cases first. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)121;

    /* Use the linear recurrence.  We only keep the last three terms. */
    seq_t a1 = (seq_t)1;   /* a(n-3)  */
    seq_t a2 = (seq_t)13;  /* a(n-2)  */
    seq_t a3 = (seq_t)121; /* a(n-1)  */
    seq_t ai   = (seq_t)0; /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 13*a(i-1) - 48*a(i-2) + 36*a(i-3) */
        ai = (seq_t)13 * a3 - (seq_t)48 * a2 + (seq_t)36 * a1;
        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return ai;
}
