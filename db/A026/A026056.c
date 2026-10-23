/*
 * A026056  a(n) = (d(n)-r(n))/5, where d = A026054 and r is the periodic
 *          sequence with fundamental period (3,3,0,0,4).
 * https://oeis.org/A026056
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) +
 *            3*a(n-7) - a(n-8) Initial values (offset 3): a(3) = 2, a(4) = 5,
 *            a(5) = 10, a(6) = 16, a(7) = 23, a(8) = 33, a(9) = 45, a(10) =
 *            60
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Provided by the build environment */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – not part of the sequence. */
    if (n < 3) return (seq_t)0;

    /* Base values for n = 3 … 10. */
    static const seq_t base[8] = {2, 5, 10, 16, 23, 33, 45, 60};

    if (n <= 10)
        return base[n - 3];

    /* Sliding window of the last eight values:
       buf[0] = a(i-8), buf[1] = a(i-7), … , buf[7] = a(i-1)  */
    seq_t buf[8] = { base[0], base[1], base[2], base[3],
                     base[4], base[5], base[6], base[7] };

    seq_t val;
    for (idx_t i = 11; i <= n; ++i) {
        /* Compute a(i) from the recurrence, using the window. */
        val = 3 * buf[7]         /* a(i-1) */
            - 3 * buf[6]         /* a(i-2) */
            +   buf[5]           /* a(i-3) */
            +   buf[3]           /* a(i-5) */
            - 3 * buf[2]         /* a(i-6) */
            + 3 * buf[1]         /* a(i-7) */
            -   buf[0];          /* a(i-8) */

        /* Shift the window left: discard a(i-8), keep a(i-7)..a(i-1). */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = val;
    }

    return val;
}
