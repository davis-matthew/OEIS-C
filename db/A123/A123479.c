/*
 * A123479  Coefficients of series giving the best rational approximations to
 *          sqrt(6).
 * https://oeis.org/A123479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 99*a(n+2) - 99*a(n+1) + a(n); a(1)=20, a(2)=1980,
 *            a(3)=194040
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n < 1, guard */
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)1980;
    if (n == 3) return (seq_t)194040;

    seq_t a0 = (seq_t)20;
    seq_t a1 = (seq_t)1980;
    seq_t a2 = (seq_t)194040;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a3 = (seq_t)99 * a2 - (seq_t)99 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
