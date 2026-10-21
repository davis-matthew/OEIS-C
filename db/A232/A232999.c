/*
 * A232999  a(n) = 6 + 4*(-1)^n + (2+sqrt(3))^n + (2-sqrt(3))^n +
 *          2*(1+sqrt(2))^n + 2*(1-sqrt(2))^n.
 * https://oeis.org/A232999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232999
   Formula: a(n) = 6 + 4*(-1)^n + (2+√3)^n + (2-√3)^n + 2*(1+√2)^n + 2*(1-√2)^n
   Recurrence (n ≥ 6):
     a(n) = 6*a(n-1) - 7*a(n-2) - 8*a(n-3) + 9*a(n-4) + 2*a(n-5) - a(n-6)
   Initial values:
     a(0)=16, a(1)=10, a(2)=36, a(3)=82, a(4)=272, a(5)=890
   The function computes the nth term iteratively using the recurrence
   and requires only a constant amount of memory.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 5 */
    static const seq_t init[6] = {16, 10, 36, 82, 272, 890};

    if (n <= 5)
        return init[(size_t)n];

    /* keep the last six values; a0 = a(n-6), a1 = a(n-5), …, a5 = a(n-1) */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t an = 6 * a5 - 7 * a4 - 8 * a3 + 9 * a2 + 2 * a1 - a0;
        /* shift the window one step forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return a5;
}
