/*
 * A042406  Numerators of continued fraction convergents to sqrt(731).
 * https://oeis.org/A042406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1460*a(n-2) - a(n-4) Initial values: a(0)=27, a(1)=730,
 *            a(2)=39447, a(3)=1065799
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)730;
    if (n == 2) return (seq_t)39447;
    if (n == 3) return (seq_t)1065799;

    seq_t a0 = (seq_t)27;
    seq_t a1 = (seq_t)730;
    seq_t a2 = (seq_t)39447;
    seq_t a3 = (seq_t)1065799;
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t curr = (seq_t)1460 * a2 - a0;  /* a(i-2)*1460 - a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
