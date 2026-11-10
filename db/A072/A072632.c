/*
 * A072632  Solutions to A072631[n]=0.
 * https://oeis.org/A072632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = ceiling(e^(n-1)), a(1) = 1 (computed exactly with
 *            fixed-point arithmetic)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* out = (x*y) >> (32*F); x, y, out have L limbs; tmp has 2L limbs */
static void A001671_mulfix(const uint32_t *x, const uint32_t *y, uint32_t *out,
                           uint32_t *tmp, size_t L, size_t F) {
    size_t i, j, ly = L;
    while (ly > 0 && y[ly - 1] == 0) ly--;
    memset(tmp, 0, 2 * L * sizeof(uint32_t));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0;
        if (x[i] == 0) continue;
        for (j = 0; j < ly; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        j = i + ly;
        while (carry && j < 2 * L) {
            uint64_t t = (uint64_t)tmp[j] + carry;
            tmp[j] = (uint32_t)t;
            carry = t >> 32;
            j++;
        }
    }
    memcpy(out, tmp + F, L * sizeof(uint32_t));
}

static seq_t A001671_a(idx_t n) {
    size_t I, F, L, i;
    uint64_t bits;
    uint32_t *e, *term, *res, *tmp;
    uint64_t k;
    int top, b;
    seq_t r;

    if (n == 0) return (seq_t)1;

    bits = (uint64_t)n * 14427 / 10000;
    I = (size_t)(bits / 32 + 3);
    F = (size_t)((bits + 128) / 32 + 2);
    L = I + F;

    e = (uint32_t *)calloc(L, sizeof(uint32_t));
    term = (uint32_t *)calloc(L, sizeof(uint32_t));
    res = (uint32_t *)calloc(L, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));

    term[F] = 1;
    e[F] = 1;
    top = (int)F;
    for (k = 1; top >= 0; k++) {
        uint64_t rem = 0;
        uint64_t carry = 0;
        int t;
        for (t = top; t >= 0; t--) {
            uint64_t cur = (rem << 32) | term[t];
            term[t] = (uint32_t)(cur / k);
            rem = cur % k;
        }
        while (top >= 0 && term[top] == 0) top--;
        if (top < 0) break;
        for (i = 0; i < L; i++) {
            uint64_t s = (uint64_t)e[i] + (i <= (size_t)top ? term[i] : 0) + carry;
            e[i] = (uint32_t)s;
            carry = s >> 32;
            if (i > (size_t)top && carry == 0) break;
        }
    }

    res[F] = 1;
    b = 63;
    while (b > 0 && !((n >> b) & 1)) b--;
    for (; b >= 0; b--) {
        A001671_mulfix(res, res, res, tmp, L, F);
        if ((n >> b) & 1) A001671_mulfix(res, e, res, tmp, L, F);
    }

    r = 0;
    for (i = L; i > F; i--) {
        uint32_t limb = res[i - 1];
        int s;
        for (s = 3; s >= 0; s--) {
            r = r * 256;
            r = r + (unsigned)((limb >> (8 * s)) & 255u);
        }
    }
    r = r + 1;

    free(e);
    free(term);
    free(res);
    free(tmp);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return A001671_a(n - 1);
}
