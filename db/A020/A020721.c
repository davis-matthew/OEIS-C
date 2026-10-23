/*
 * A020721  Pisot sequences E(7,10), P(7,10).
 * https://oeis.org/A020721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) (n ≥ 4) Initial
 *            values: a(0)=7, a(1)=10, a(2)=14, a(3)=20
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 53, u64 n <= 111, u128 n <= 227,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the known initial segment. */
    if (n == (idx_t)0) return (seq_t)7;
    if (n == (idx_t)1) return (seq_t)10;
    if (n == (idx_t)2) return (seq_t)14;
    if (n == (idx_t)3) return (seq_t)20;

    /* Working variables for the last four terms:
         a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)7;   /* a(0) */
    seq_t a1 = (seq_t)10;  /* a(1) */
    seq_t a2 = (seq_t)14;  /* a(2) */
    seq_t a3 = (seq_t)20;  /* a(3) */
    seq_t an = 0;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)( (2 * a3) - a2 + a1 - a0 );
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
