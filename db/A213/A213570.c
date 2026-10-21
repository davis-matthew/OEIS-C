/*
 * A213570  Antidiagonal sums of the convolution array A213566.
 * https://oeis.org/A213570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 6*a(n-3) + a(n-4) - 3*a(n-5) +
 *            a(n-6), a(1) = 1, a(2) = 9, a(3) = 37, a(4) = 110, a(5) = 272,
 *            a(6) = 598
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 36, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    static const seq_t init[6] = { (seq_t)1, (seq_t)9, (seq_t)37,
                                   (seq_t)110, (seq_t)272, (seq_t)598 };

    if (n <= 6) return init[n - 1];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute next term without unsigned underflow:
           next = (5*a5 + 6*a3 + a2 + a0) - (9*a4 + 3*a1). */
        seq_t sum1 = (seq_t)5 * a5 + (seq_t)6 * a3 + a2 + a0;
        seq_t sum2 = (seq_t)9 * a4 + (seq_t)3 * a1;
        seq_t next = sum1 - sum2;

        /* Shift the window for the next iteration. */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }

    return a5;
}
