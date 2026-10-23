/*
 * A021079  Expansion of 1/((1-x)(1-2x)(1-4x)(1-7x)).
 * https://oeis.org/A021079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 63*a(n-2) + 106*a(n-3) - 56*a(n-4) (n ≥ 4)
 *            Initial values: a(0) = 1, a(1) = 14, a(2) = 133, a(3) = 1086
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)14;
        case 2: return (seq_t)133;
        case 3: return (seq_t)1086;
        default: break;
    }

    seq_t a0 = (seq_t)1;        /* a(n-4)  */
    seq_t a1 = (seq_t)14;       /* a(n-3)  */
    seq_t a2 = (seq_t)133;      /* a(n-2)  */
    seq_t a3 = (seq_t)1086;     /* a(n-1)  */
    seq_t a_cur;

    for (idx_t i = 4; i <= n; ++i) {
        a_cur = a3 * (seq_t)14
                - a2 * (seq_t)63
                + a1 * (seq_t)106
                - a0 * (seq_t)56;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_cur;
    }
    return a3;
}
