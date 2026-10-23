/*
 * A041581  Denominators of continued fraction convergents to sqrt(308).
 * https://oeis.org/A041581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 702*a(n-6) - a(n-12) for n > 11 Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=9, a(4)=11, a(5)=20, a(6)=691, a(7)=711,
 *            a(8)=1402, a(9)=6319, a(10)=7721, a(11)=14040
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[12] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)9,
    (seq_t)11, (seq_t)20, (seq_t)691, (seq_t)711,
    (seq_t)1402,(seq_t)6319,(seq_t)7721,(seq_t)14040
};

seq_t a(idx_t n)
{
    if (n < 12) return init[(size_t)n];

    size_t i;
    seq_t buf[12];
    for (i = 0; i < 12; ++i) buf[i] = init[i];

    for (i = 12; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)702 * buf[(i - 6) % 12] - buf[i % 12];
        buf[i % 12] = val;
    }

    return buf[(size_t)n % 12];
}
