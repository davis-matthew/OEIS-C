/*
 * A163012  Number of n X 3 binary arrays with all 1s connected and a path of
 *          1s from top row to lower right corner.
 * https://oeis.org/A163012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + 6*a(n-3) + a(n-4) - 7*a(n-5) +
 *            a(n-6) * Initial values: * a(1) = 3 * a(2) = 16 * a(3) = 82 *
 *            a(4) = 416 * a(5) = 2103 * a(6) = 10633 * The sequence is
 *            defined for n >= 1; values for n < 1 are undefined
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle offset: a(1) is first term; for n=0 return 0 (undefined region). */
    if (n == 0) return 0;

    /* Initial 6 terms. */
    static const seq_t init[6] = {3, 16, 82, 416, 2103, 10633};

    if (n <= 6) {
        return init[n - 1];
    }

    /* Keep the last six values: a(n-5) .. a(n) */
    seq_t a1 = init[0];   /* a(n-5) */
    seq_t a2 = init[1];   /* a(n-4) */
    seq_t a3 = init[2];   /* a(n-3) */
    seq_t a4 = init[3];   /* a(n-2) */
    seq_t a5 = init[4];   /* a(n-1) */
    seq_t a6 = init[5];   /* a(n)   */

    idx_t i;
    for (i = 7; i <= n; ++i) {
        /* Compute a(i) from the recurrence:
         * a(i) = 7*a6 - 11*a5 + 6*a4 + a3 - 7*a2 + a1
         * Reordered to avoid negative intermediates:
         * left  = 7*a6 + 6*a4 + a3 + a1
         * right = 11*a5 + 7*a2
         * a(i)  = left - right
         */
        seq_t left  = a6 * 7 + a4 * 6 + a3 + a1;
        seq_t right = a5 * 11 + a2 * 7;
        seq_t next  = left - right;

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
