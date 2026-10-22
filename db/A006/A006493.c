/*
 * A006493  Generalized Lucas numbers.
 * https://oeis.org/A006493
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 10*a(n-3) + 15*a(n-4) + 11*a(n-5) -
 *            15*a(n-6) - 10*a(n-7) + 5*a(n-8) + 5*a(n-9) + a(n-10), a(3..12)
 *            = 1, 0, 6, 7, 28, 54, 135, 286, 627, 1313 (g.f. denominator
 *            (1-x-x^2)^5)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A006493_init[10] = { 1, 0, 6, 7, 28, 54, 135, 286, 627, 1313 };

seq_t a(idx_t n) {
    seq_t w[10];
    idx_t i;
    int k;
    if (n < 3) return (seq_t)0;
    for (k = 0; k < 10; k++) {
        int v = A006493_init[k];
        w[k] = (seq_t)v;
    }
    if (n < 13) return w[n - 3];
    for (i = 13; i <= n; i++) {
        /* w[9] = a(i-1), w[8] = a(i-2), ..., w[0] = a(i-10) */
        seq_t pos = 5 * w[9] + 15 * w[6] + 11 * w[5] + 5 * w[2] + 5 * w[1] + w[0];
        seq_t neg = 5 * w[8] + 10 * w[7] + 15 * w[4] + 10 * w[3];
        seq_t next = pos - neg;
        for (k = 0; k < 9; k++) w[k] = w[k + 1];
        w[9] = next;
    }
    return w[9];
}
