/*
 * A042924  Numerators of continued fraction convergents to sqrt(994).
 * https://oeis.org/A042924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2270*a(n-6) - a(n-12) Initial values: a(0)=31, a(1)=32,
 *            a(2)=63, a(3)=536, a(4)=599, a(5)=1135, a(6)=70969, a(7)=72104,
 *            a(8)=143073, a(9)=1216688, a(10)=1359761, a(11)=2576449
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial sequence values */
    static const seq_t init[12] = {
        31, 32, 63, 536, 599, 1135,
        70969, 72104, 143073, 1216688,
        1359761, 2576449
    };

    if (n < 12)
        return init[(size_t)n];

    /* Sliding window of the last 12 computed values   */
    seq_t buf[12];
    size_t i;
    for (i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute from a(12) up to a(n) */
    for (i = 12; i <= (size_t)n; ++i) {
        seq_t val = 2270 * buf[6] - buf[0];   /* a(i) = 2270*a(i-6) - a(i-12) */
        /* Shift window left by one position */
        for (size_t j = 0; j < 11; ++j)
            buf[j] = buf[j + 1];
        buf[11] = val;
    }

    return buf[11];   /* a(n) */
}
