/*
 * A042081  Denominators of continued fraction convergents to sqrt(564).
 * https://oeis.org/A042081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A042081: Denominators of continued fraction convergents to sqrt(564)
 * Recurrence: a(n) = 190*a(n-4) - a(n-8)  for n > 7
 * Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=187, a(5)=191,
 *                 a(6)=569, a(7)=760
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Predefined first eight terms */
    static const seq_t init[8] = {1, 1, 3, 4, 187, 191, 569, 760};

    if (n < 8)
        return init[n];

    /* Circular buffer holding the last 8 computed terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t val;
    for (idx_t i = 8; i <= n; ++i) {
        /* a(i) = 190*a(i-4) - a(i-8) */
        val = 190 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = val; /* store newest term in circular buffer */
    }
    return val;
}
