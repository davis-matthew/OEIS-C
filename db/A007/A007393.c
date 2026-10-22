/*
 * A007393  Number of strict 5th-order maximal independent sets in cycle
 *          graph.
 * https://oeis.org/A007393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x^14*(7-5*x^2)/(1-3*x^2+3*x^4-x^6-x^7+2*x^9-x^11), i.e.
 *            a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) + a(n-7) - 2*a(n-9) +
 *            a(n-11) for n >= 17, with a(1..13) = 0, a(14) = 7, a(15) = 0,
 *            a(16) = 16
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A007393_W 12

seq_t a(idx_t n) {
    seq_t w[A007393_W];
    idx_t i;
    if (n < 1) return 0;
    for (i = 0; i < A007393_W; i++) w[i] = 0;
    for (i = 1; i <= n; i++) {
        seq_t v;
        if (i < 14) {
            v = 0;
        } else if (i == 14) {
            v = 7;
        } else if (i == 15) {
            v = 0;
        } else if (i == 16) {
            v = 16;
        } else {
            seq_t pos = 3 * w[(i - 2) % A007393_W] + w[(i - 6) % A007393_W]
                      + w[(i - 7) % A007393_W] + w[(i - 11) % A007393_W];
            seq_t neg = 3 * w[(i - 4) % A007393_W] + 2 * w[(i - 9) % A007393_W];
            v = pos - neg;
        }
        w[i % A007393_W] = v;
    }
    return w[n % A007393_W];
}
