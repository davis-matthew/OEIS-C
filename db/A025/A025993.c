/*
 * A025993  Expansion of 1/((1-2*x)*(1-5*x)*(1-7*x)*(1-9*x)).
 * https://oeis.org/A025993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 185*a(n-2) + 601*a(n-3) - 630*a(n-4) a(0)=1,
 *            a(1)=23, a(2)=344, a(3)=4258
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)344;
    if (n == 3) return (seq_t)4258;

    /* Start recurrence with the known initial values. */
    seq_t a0 = (seq_t)1;       /* a(n-4) */
    seq_t a1 = (seq_t)23;      /* a(n-3) */
    seq_t a2 = (seq_t)344;     /* a(n-2) */
    seq_t a3 = (seq_t)4258;    /* a(n-1) */

    /* Compute up to index n. */
    for (idx_t i = 4; i <= n; ++i) {
        /* Recurrence: a(i) = 23*a3 - 185*a2 + 601*a1 - 630*a0 */
        seq_t ai = (seq_t)23 * a3 - (seq_t)185 * a2 + (seq_t)601 * a1 - (seq_t)630 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
