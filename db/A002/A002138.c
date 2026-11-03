/*
 * A002138  6th powers written backwards.
 * https://oeis.org/A002138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = R(n^6), where R reverses the decimal digits; n^6 is
 *            formed in base-10^9 limbs so it never overflows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 32, u64 n <= 1467,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9330 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static int a002138_mul(const uint64_t *x, int lx, const uint64_t *y, int ly, uint64_t *out) {
    int i, j, L = lx + ly;
    for (i = 0; i < L; i++) out[i] = 0;
    for (i = 0; i < lx; i++) {
        uint64_t carry = 0;
        for (j = 0; j < ly; j++) {
            uint64_t t = out[i + j] + x[i] * y[j] + carry;
            out[i + j] = t % 1000000000ULL;
            carry = t / 1000000000ULL;
        }
        int k = i + ly;
        while (carry) {
            uint64_t t = out[k] + carry;
            out[k] = t % 1000000000ULL;
            carry = t / 1000000000ULL;
            k++;
        }
    }
    while (L > 1 && out[L - 1] == 0) L--;
    return L;
}

seq_t a(idx_t n) {
    uint64_t b[4], p2[8], p4[16], p6[32];
    int lb = 0, l2, l4, l6, i, k;
    uint64_t v = (uint64_t)n;
    seq_t r = 0;
    int started = 0;
    if (v == 0) return 0;
    while (v) { b[lb++] = v % 1000000000ULL; v /= 1000000000ULL; }
    l2 = a002138_mul(b, lb, b, lb, p2);
    l4 = a002138_mul(p2, l2, p2, l2, p4);
    l6 = a002138_mul(p4, l4, p2, l2, p6);
    for (i = 0; i < l6; i++) {
        uint64_t limb = p6[i];
        int ndig = (i == l6 - 1) ? 0 : 9;
        if (i == l6 - 1) {
            uint64_t t = limb;
            while (t) { ndig++; t /= 10; }
        }
        for (k = 0; k < ndig; k++) {
            unsigned d = (unsigned)(limb % 10);
            limb /= 10;
            if (!started && d == 0) continue;
            started = 1;
            r = r * 10 + d;
        }
    }
    return r;
}
