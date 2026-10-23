/*
 * A192910  Coefficient of x in the reduction by (x^2 -> x + 1) of the
 *          polynomial p(n,x) defined below at Comments.
 * https://oeis.org/A192910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) - a(n-5) Recurrence is valid
 *            for n ≥ 5 with base values a(0)=0, a(1)=1, a(2)=4, a(3)=13,
 *            a(4)=42
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of sequence A192910 (OEIS). */
seq_t a(idx_t n)
{
    /* Base values up to index 4 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)42;

    /* Keep the last five computed values */
    seq_t a0 = (seq_t)0;  /* a(n-5)  */
    seq_t a1 = (seq_t)1;  /* a(n-4)  */
    seq_t a2 = (seq_t)4;  /* a(n-3)  */
    seq_t a3 = (seq_t)13; /* a(n-2)  */
    seq_t a4 = (seq_t)42; /* a(n-1)  */
    seq_t a5;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) - a(n-5) */
        a5 = a4 * (seq_t)4 - a3 * (seq_t)3 + a2 - a0;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }

    return a4; /* after loop, a4 holds a(n) */
}
