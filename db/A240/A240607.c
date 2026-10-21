/*
 * A240607  a(n) = 2*a(n-2) + a(n-3) + a(n-4) for n>=4, a(n) = binomial(n,3)
 *          for n<4.
 * https://oeis.org/A240607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) + a(n-4) for n>=4, a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 45, u64 n <= 87, u128 n <= 169,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)0; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t a2 = (seq_t)0; /* a(2) */
    seq_t a3 = (seq_t)1; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 2 * a2 + a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
