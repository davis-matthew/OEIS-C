/*
 * A099489  Expansion of (1-x+x^2)/((1+x^2)(1-4x+x^2)).
 * https://oeis.org/A099489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A099489
   a(0) = 1, a(1) = 3, a(2) = 11, a(3) = 42
   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) - a(n-4)   for n >= 4
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)42;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)11;
    seq_t a3 = (seq_t)42;
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = a3 * (seq_t)4
            - a2 * (seq_t)2
            + a1 * (seq_t)4
            - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
