/*
 * A021234  Expansion of 1/((1-x)(1-2x)(1-7x)(1-10x)).
 * https://oeis.org/A021234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Expansion of 1/((1-x)(1-2x)(1-7x)(1-10x))
   Formula (recurrence): a(n) = 20*a(n-1) - 123*a(n-2) + 244*a(n-3) - 140*a(n-4) for n > 3
   Initial values: a(0)=1, a(1)=20, a(2)=277, a(3)=3324 */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)277;
    if (n == 3) return (seq_t)3324;

    /* Four previous terms */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)20;     /* a(n-3) */
    seq_t a2 = (seq_t)277;    /* a(n-2) */
    seq_t a3 = (seq_t)3324;   /* a(n-1) */
    seq_t an = 0;

    /* Iterate up to n (for n >= 4) */
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)20 * a3
            - (seq_t)123 * a2
            + (seq_t)244 * a1
            - (seq_t)140 * a0;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
