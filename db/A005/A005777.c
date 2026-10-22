/*
 * A005777  Rook polynomials.
 * https://oeis.org/A005777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + a(n-3) - 2*a(n-4) + 10*a(n-5) -
 *            2*a(n-6) - 2*a(n-7) - 5*a(n-8) + a(n-9) + a(n-10) + a(n-11) for
 *            n >= 15, with a(1..14) = 1, 4, 18, 96, 265, 672, 1617, 3776,
 *            8577, 19080, 41745, 90048, 191945, 404992. (Denominator
 *            (1-x)^2*(1-x-x^2-x^3)^3 of the conjectured g.f.; the li...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const unsigned long init_terms[14] = {
    1UL, 4UL, 18UL, 96UL, 265UL, 672UL, 1617UL, 3776UL, 8577UL,
    19080UL, 41745UL, 90048UL, 191945UL, 404992UL
};

seq_t a(idx_t n) {
    seq_t w[14];
    idx_t i, k;
    if (n == 0) return (seq_t)0;
    if (n <= 14) return (seq_t)init_terms[n - 1];
    for (i = 0; i < 14; i++) w[i] = (seq_t)init_terms[i];
    /* w[13] = a(k-1), w[12] = a(k-2), ... */
    for (k = 15; k <= n; k++) {
        seq_t pos = (seq_t)5 * w[13] + w[11] + (seq_t)10 * w[9]
                  + w[5] + w[4] + w[3];
        seq_t neg = (seq_t)7 * w[12] + (seq_t)2 * w[10] + (seq_t)2 * w[8]
                  + (seq_t)2 * w[7] + (seq_t)5 * w[6];
        seq_t next = pos - neg;
        for (i = 0; i < 13; i++) w[i] = w[i + 1];
        w[13] = next;
    }
    return w[13];
}
