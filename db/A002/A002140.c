/*
 * A002140  7th powers written backwards.
 * https://oeis.org/A002140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = R(n^7), where R reverses the decimal digits. n^7 is built
 *            exactly as an array of decimal digits, and the reversed number
 *            is then accumulated in seq_t
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 517,
 *            u128 n <= 10000, big any n
 * Helpers:   a002140_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002140_mul.h"

seq_t a(idx_t n) {
    unsigned char base[32], cur[320], nxt[320];
    int lb = 0, lc, k, i;
    idx_t m = n;
    seq_t r = 0;
    if (m == 0) base[lb++] = 0;
    while (m > 0) { base[lb++] = (unsigned char)(m % 10); m /= 10; }
    for (i = 0; i < lb; i++) cur[i] = base[i];
    lc = lb;
    for (k = 1; k < 7; k++) {
        lc = a002140_mul(cur, lc, base, lb, nxt);
        for (i = 0; i < lc; i++) cur[i] = nxt[i];
    }
    /* cur holds n^7, least significant digit first; read it forwards to reverse */
    for (i = 0; i < lc; i++) r = r * 10 + (seq_t)cur[i];
    return r;
}
