/*
 * A202391  Indices of the smallest of four consecutive triangular numbers
 *          summing up to a square.
 * https://oeis.org/A202391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the index of the first of four consecutive triangular
 *            numbers whose sum is a perfect square. It satisfies a(1)=5,
 *            a(2)=39, a(3)=237 and the recurrence a(n+2) = 6*a(n+1) - a(n) +
 *            8 for n ≥ 1. The recurrence uses only integer additions,
 *            subtractions, and multiplication by 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, return 0 for safety */

    /* Initial values a(1)=5, a(2)=39 */
    seq_t a1 = (seq_t)5;
    if (n == 1)
        return a1;

    seq_t a2 = (seq_t)39;
    if (n == 2)
        return a2;

    /* Iterate using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        /* m = 6 * a2 */
        seq_t m = a2 * (seq_t)6;
        /* next = m - a1 + 8 */
        seq_t next = m - a1 + (seq_t)8;
        /* Shift variables for next iteration */
        a1 = a2;
        a2 = next;
    }
    return a2;
}
