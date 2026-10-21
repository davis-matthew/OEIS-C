/*
 * A082630  Limit of the sequence obtained from S(0) = (1,1) and, for n > 0,
 *          S(n) = I(S(n-1)), where I consists of inserting, for i = 1, 2,
 *          3..., the term a(i) + a(i+1) between any two terms for which
 *          7*a(i+1) <= 11*a(i).
 * https://oeis.org/A082630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) - a(n-4) for n ≥ 5 with initial values a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=8
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return known initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)8;

    /* Sliding window for the recurrence: a1 = a(k-4), a2 = a(k-3),
       a3 = a(k-2), a4 = a(k-1). */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)5;
    seq_t a4 = (seq_t)8;
    seq_t an;

    for (idx_t k = 5; k <= n; ++k) {
        an = (seq_t)4 * a3 - a1;      /* a(k) = 4*a(k-2) - a(k-4) */
        /* Shift the window forward. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
