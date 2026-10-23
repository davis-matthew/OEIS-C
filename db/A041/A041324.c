/*
 * A041324  Numerators of continued fraction convergents to sqrt(176).
 * https://oeis.org/A041324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041324: Numerators of convergents to sqrt(176)
    Formula:
    a(n) = 398*a(n-4) - a(n-8)   for n > 7
    Initial values:
    a(0)=13, a(1)=40, a(2)=53, a(3)=199,
    a(4)=5227, a(5)=15880, a(6)=21107, a(7)=79201
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)13,  (seq_t)40,   (seq_t)53,   (seq_t)199,
        (seq_t)5227, (seq_t)15880, (seq_t)21107, (seq_t)79201
    };

    if (n < (idx_t)8u)            /* small indices */
        return init[(size_t)n];

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t term = (seq_t)398 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = term;
    }

    return buf[n & 7];
}
