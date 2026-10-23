/*
 * A159675  Expansion of x*(1 + x)/(1 - 32*x + x^2).
 * https://oeis.org/A159675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=33; a(n)=32*a(n-1)-a(n-2) (n ≥ 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A159675  – Expansion of x*(1 + x)/(1 - 32*x + x^2)     */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)33;

    seq_t prev2 = (seq_t)1;   /* a(1)   */
    seq_t prev1 = (seq_t)33;  /* a(2)   */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)(32) * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
