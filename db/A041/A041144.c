/*
 * A041144  Numerators of continued fraction convergents to sqrt(82).
 * https://oeis.org/A041144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 18*a(n-1) + a(n-2), a(0) = 9, a(1) = 163
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)9;
    if (n == 1) return (seq_t)163;

    seq_t prev2 = (seq_t)9;   /* a(0) */
    seq_t prev1 = (seq_t)163; /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)18 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
