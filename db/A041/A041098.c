/*
 * A041098  Numerators of continued fraction convergents to sqrt(57).
 * https://oeis.org/A041098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041098 – Numerators of continued fraction convergents to sqrt(57)
   Recurrence: a(n) = 302*a(n-6) - a(n-12)  (n ≥ 12)
   Initial values:
   a(0)=7,  a(1)=8,  a(2)=15,  a(3)=68,  a(4)=83,
   a(5)=151, a(6)=2197, a(7)=2348, a(8)=4545, a(9)=20528,
   a(10)=25073, a(11)=45601
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)7,   (seq_t)8,   (seq_t)15,  (seq_t)68,
        (seq_t)83,  (seq_t)151, (seq_t)2197, (seq_t)2348,
        (seq_t)4545,(seq_t)20528,(seq_t)25073,(seq_t)45601
    };

    if (n < 12) return init[(size_t)n];

    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i) buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        size_t idx1 = (size_t)((i - 6) % 12);
        size_t idx2 = (size_t)((i - 12) % 12);
        seq_t val = (seq_t)302 * buf[idx1] - buf[idx2];
        buf[(size_t)(i % 12)] = val;
    }

    return buf[(size_t)(n % 12)];
}
