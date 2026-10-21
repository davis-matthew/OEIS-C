/*
 * A228478  Number of n X 4 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically or antidiagonally.
 * https://oeis.org/A228478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 15*a(n-3) + 4*a(n-4) - 6*a(n-5) -
 *            a(n-6) + 3*a(n-7) - a(n-8) Initial values (1‑based indexing):
 *            a(1)=3, a(2)=9, a(3)=41, a(4)=172, a(5)=728, a(6)=3084,
 *            a(7)=13050, a(8)=55252
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Base values for n = 1 … 8 */
    static const seq_t base[8] = {
        (seq_t)3,
        (seq_t)9,
        (seq_t)41,
        (seq_t)172,
        (seq_t)728,
        (seq_t)3084,
        (seq_t)13050,
        (seq_t)55252
    };

    if (n < 1)             /* sequence is defined for n ≥ 1 only */
        return (seq_t)0;

    if (n <= 8)            /* return the pre‑computed value */
        return base[n - 1];

    /* Compute iteratively using the recurrence.
       We keep only the last eight values in a circular buffer. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    seq_t val;
    for (idx_t k = 9; k <= n; ++k) {
        /* recurrence using the stored last eight values:
           buf[0] = a(k-8), buf[1] = a(k-7), …, buf[7] = a(k-1) */
        val = buf[7] + 10 * buf[6] + 15 * buf[5] + 4 * buf[4]
              - 6 * buf[3] - buf[2] + 3 * buf[1] - buf[0];

        /* shift the window left: drop a(k-8) and add a(k) */
        for (size_t i = 0; i < 7; ++i)
            buf[i] = buf[i + 1];
        buf[7] = val;
    }
    return val;   /* val now holds a(n) */
}
