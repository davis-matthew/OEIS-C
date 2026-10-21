/*
 * A238518  Number of (n+1) X (1+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A238518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 21*a(n-2) - 14*a(n-3); a(1) = 84, a(2) = 588,
 *            a(3) = 7056
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset 1: no term for n == 0, but return 0 harmlessly */
    if (n == 0)
        return (seq_t)0;

    /* Initial terms */
    if (n == 1) return (seq_t)84;
    if (n == 2) return (seq_t)588;
    if (n == 3) return (seq_t)7056;

    /* Compute the sequence iteratively */
    seq_t a1 = (seq_t)84;   /* a(n-3) */
    seq_t a2 = (seq_t)588;  /* a(n-2) */
    seq_t a3 = (seq_t)7056; /* a(n-1) */
    seq_t a_next;

    for (idx_t i = 4; i <= n; ++i) {
        a_next = (seq_t)9 * a3;
        a_next += (seq_t)21 * a2;
        a_next -= (seq_t)14 * a1;
        /* slide window */
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}
