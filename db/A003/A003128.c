/*
 * A003128  Number of driving-point impedances of an n-terminal network.
 * https://oeis.org/A003128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=2..n} binomial(k,2)*Stirling2(n,k) (Stirling
 *            transform of A000217)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A_grow.h"

static seq_t *buf = 0;
static idx_t buflen = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    seq_t sum;
    if (buflen < n + 1) {
        buf = A_grow(buf, buflen, n + 1);
        buflen = n + 1;
    }
    /* row of Stirling numbers of the second kind, built in place */
    buf[0] = 1;
    for (k = 1; k <= n; k++) buf[k] = 0;
    for (i = 1; i <= n; i++) {
        for (k = i; k >= 1; k--) {
            buf[k] = buf[k] * (seq_t)k + buf[k - 1];
        }
        buf[0] = 0;
    }
    sum = 0;
    for (k = 2; k <= n; k++) {
        idx_t c = k * (k - 1) / 2;
        sum += buf[k] * (seq_t)c;
    }
    return sum;
}
