/*
 * A126530  Number of base 9 n-digit numbers with adjacent digits differing by
 *          five or less.
 * https://oeis.org/A126530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) - 4*a(n-3) - 3*a(n-4) for n>3 Initial
 *            values: a(0)=1, a(1)=9, a(2)=69, a(3)=541
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Classical recurrence, no dynamic allocation needed. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    if (n == 0) return a0;

    seq_t a1 = (seq_t)9;   /* a(1) */
    if (n == 1) return a1;

    seq_t a2 = (seq_t)69;  /* a(2) */
    if (n == 2) return a2;

    seq_t a3 = (seq_t)541; /* a(3) */
    if (n == 3) return a3;

    /* Forward compute up to a(n) using the recurrence. */
    seq_t a_prev4 = a0;    /* a(i-4) */
    seq_t a_prev3 = a1;    /* a(i-3) */
    seq_t a_prev2 = a2;    /* a(i-2) */
    seq_t a_prev1 = a3;    /* a(i-1) */
    seq_t current = 0;

    for (idx_t i = 4; i <= n; ++i) {
        current = 7 * a_prev1 + 7 * a_prev2 - 4 * a_prev3 - 3 * a_prev4;
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = current;
    }

    return current;
}
