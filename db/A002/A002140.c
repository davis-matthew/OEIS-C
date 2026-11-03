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
 * Validated: standard, pass, 44 terms through n = 9999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a002140_mul(const unsigned char *x, int lx, const unsigned char *y, int ly,
                       unsigned char *out) {
    uint64_t tmp[320];
    int i, j, L = lx + ly;
    uint64_t carry = 0;
    for (i = 0; i < L + 2; i++) tmp[i] = 0;
    for (i = 0; i < lx; i++)
        for (j = 0; j < ly; j++)
            tmp[i + j] += (uint64_t)x[i] * y[j];
    for (i = 0; i < L; i++) {
        uint64_t v = tmp[i] + carry;
        out[i] = (unsigned char)(v % 10);
        carry = v / 10;
    }
    while (L > 1 && out[L - 1] == 0) L--;
    return L;
}

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
