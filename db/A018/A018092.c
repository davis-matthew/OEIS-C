/*
 * A018092  Expansion of 1/((1-3*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A018092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 171*a(n-2) + 324*a(n-3); a(0)=1, a(1)=24,
 *            a(2)=405
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)405;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)24;  /* a(1) */
    seq_t a2 = (seq_t)405; /* a(2) */
    seq_t an = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 * (seq_t)24;
        an -= a1 * (seq_t)171;
        an += a0 * (seq_t)324;

        /* slide window */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
