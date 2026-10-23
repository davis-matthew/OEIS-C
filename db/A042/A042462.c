/*
 * A042462  Numerators of continued fraction convergents to sqrt(759).
 * https://oeis.org/A042462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042462
   Numerators of continued fraction convergents to sqrt(759).
   Recurrence: a(n) = 1102*a(n-6) - a(n-12)  for n ≥ 12
   Initial values: a(0)=27, a(1)=28, a(2)=55, a(3)=248, a(4)=303, a(5)=551,
                    a(6)=30057, a(7)=30608, a(8)=60665, a(9)=273268,
                    a(10)=333933, a(11)=607201
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 12 terms */
    static const seq_t init[12] = {
        27,   28,   55,   248,   303,   551,
        30057,30608,60665,273268,333933,607201
    };

    if (n < 12) {
        return init[(size_t)n];
    }

    /* Local circular buffer of length 12 */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i) {
        buf[i] = init[i];
    }

    for (size_t i = 12; i <= (size_t)n; ++i) {
        size_t idx6  = (i - 6)  % 12;
        size_t idx12 = (i - 12) % 12;
        buf[i % 12] = (seq_t)1102 * buf[idx6] - buf[idx12];
    }

    return buf[(size_t)n % 12];
}
