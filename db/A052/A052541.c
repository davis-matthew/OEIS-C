/*
 * A052541  Expansion of 1/(1-3*x-x^3).
 * https://oeis.org/A052541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3), a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)3;   /* a(n-2) */
    seq_t a2 = (seq_t)9;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 * (seq_t)3 + a0; /* a(i) = 3*a(i-1) + a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
