/*
 * A041945  Denominators of continued fraction convergents to sqrt(495).
 * https://oeis.org/A041945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 178*a(n-2) - a(n-4) for n>3 initial values: a(0)=1,
 *            a(1)=4, a(2)=177, a(3)=712
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for indices 0..3 */
    static const seq_t init[4] = {(seq_t)1, (seq_t)4, (seq_t)177, (seq_t)712};

    if ((idx_t)n < 4)
        return init[(size_t)n];

    /* Circular buffer holding the last four computed terms */
    seq_t buf[4];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];

    for (idx_t i = 4; i <= n; ++i) {
        /* indices modulo 4 give the required previous terms */
        idx_t idx2 = (i - 2) & 3U;
        idx_t idx4 = (i - 4) & 3U;
        seq_t val = (seq_t)178 * buf[(size_t)idx2] - buf[(size_t)idx4];
        buf[(size_t)(i & 3U)] = val;
    }

    return buf[(size_t)(n & 3U)];
}
