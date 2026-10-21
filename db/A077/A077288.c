/*
 * A077288  First member of the Diophantine pair (m,k) that satisfies 6(m^2 +
 *          m) = k^2 + k: a(n) = m.
 * https://oeis.org/A077288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-2) - a(n-4) + 4 (for n > 4) with initial values
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=14, a(4)=34
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {0, 1, 3, 14, 34};
    if (n < 5)
        return init[n];

    /* circular buffer of the last five terms:
       buf[0] = a[i-4], buf[1] = a[i-3], buf[2] = a[i-2],
       buf[3] = a[i-1], buf[4] = a[i]   (at loop start). */
    seq_t buf[5];
    for (size_t k = 0; k < 5; ++k)
        buf[k] = init[k];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = 10 * buf[3] - buf[1] + 4;  /* a(i-2) = buf[3], a(i-4) = buf[1] */

        /* shift buffer left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = new_val;
    }

    return buf[4];   /* a(n) */
}
