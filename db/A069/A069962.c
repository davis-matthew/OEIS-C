/*
 * A069962  Define C(n) by the recursion C(0) = 5*i where i^2 = -1, C(n+1) =
 *          1/(1 + C(n)), then a(n) = 5*(-1)^n/Im(C(n)) where Im(z) denotes
 *          the imaginary part of z.
 * https://oeis.org/A069962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3); a(0)=1, a(1)=26, a(2)=29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)29;

    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)26;  /* a(n-2)  */
    seq_t a2 = (seq_t)29;  /* a(n-1)  */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (a2 << 1) + (a1 << 1) - a0;   /* 2*a2 + 2*a1 - a0 */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
