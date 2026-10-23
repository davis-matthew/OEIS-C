/*
 * A164749  Number of n X 3 1..4 arrays with all 1s connected, all 2s
 *          connected, all 3s connected, all 4s connected, 1 in the upper left
 *          corner, 2 in the upper right corner, 3 in the lower left corner,
 *          and 4 in the lower right corner.
 * https://oeis.org/A164749
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (n >= 11) a(n) = 12*a(n-1) - 63*a(n-2) +
 *            190*a(n-3) - 363*a(n-4) + 456*a(n-5) - 377*a(n-6) + 198*a(n-7) -
 *            60*a(n-8) + 8*a(n-9) Initial values: a(2)=8, a(3)=108, a(4)=730,
 *            a(5)=3576, a(6)=14414, a(7)=50928, a(8)=163722, a(9)=490496,
 *            a(10)=1391862
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 47, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* For n <= 10 use the table of initial values directly. */
    if (n <= 10) {
        static const seq_t init[11] = {
            0, 0, 8, 108, 730, 3576, 14414,
            50928, 163722, 490496, 1391862
        };
        return init[n];
    }

    /* Allocate a buffer big enough to hold all values up to n. */
    seq_t *buf = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!buf) return 0;  /* Failure – return zero (unlikely in tests). */

    /* Set the leading zeros and the known initial values. */
    buf[0] = 0;
    buf[1] = 0;
    static const seq_t init[11] = {
        0, 0, 8, 108, 730, 3576, 14414,
        50928, 163722, 490496, 1391862
    };
    for (idx_t i = 2; i <= 10; ++i) buf[i] = init[i];

    /* Compute using the recurrence for n >= 11. */
    for (idx_t k = 11; k <= n; ++k) {
        buf[k] =
            (seq_t)12 * buf[k-1]
            - (seq_t)63 * buf[k-2]
            + (seq_t)190 * buf[k-3]
            - (seq_t)363 * buf[k-4]
            + (seq_t)456 * buf[k-5]
            - (seq_t)377 * buf[k-6]
            + (seq_t)198 * buf[k-7]
            - (seq_t)60  * buf[k-8]
            + (seq_t)8   * buf[k-9];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
