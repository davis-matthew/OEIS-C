/*
 * A082176  Professor E. P. B. Umbugio's sequence.
 * https://oeis.org/A082176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1492^n - 1770^n - 1863^n + 2141^n Recurrence: a(0)=0,
 *            a(1)=0, a(2)=206276, a(3)=1124101062,
 *            a(n)=7266*a(n-1)-19690571*a(n-2)+23585007306*a(n-3)-10533473613720*a(n-4)
 *            for n>3
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)206276;
    if (n == 3)
        return (seq_t)1124101062;

    /* Recurrence constants */
    static const seq_t c1 = (seq_t)7266ULL;
    static const seq_t c2 = (seq_t)19690571ULL;
    static const seq_t c3 = (seq_t)23585007306ULL;
    static const seq_t c4 = (seq_t)10533473613720ULL;

    /* Iterative computation */
    seq_t a0 = (seq_t)0;         /* a(n-4) */
    seq_t a1 = (seq_t)0;         /* a(n-3) */
    seq_t a2 = (seq_t)206276;    /* a(n-2) */
    seq_t a3 = (seq_t)1124101062;/* a(n-1) */
    seq_t an;                   

    for (idx_t i = 4; i <= n; ++i) {
        an = c1 * a3 - c2 * a2 + c3 * a1 - c4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
