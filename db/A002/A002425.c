/*
 * A002425  Denominator of Pi^(2n)/(Gamma(2n)*(1-2^(-2n))*zeta(2n)).
 * https://oeis.org/A002425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = odd part of the tangent number A000182(n) =
 *            A000182(n)/2^(2n-2-A007814(n)), tangent numbers by the
 *            Brent-Zimmermann recurrence T(j) = (j-k)*T(j-1) + (j-k+2)*T(j)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   tz_limbs, tz_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/tz_limbs.h"
#include "../utils/tz_width.h"

static uint32_t *tz_data = 0;
static size_t *tz_off = 0;
static size_t *tz_len = 0;
static idx_t tz_cap = 0;

static void tz_build(idx_t N) {
    idx_t j, k;
    size_t i, total = 0;
    free(tz_data); free(tz_off); free(tz_len);
    tz_off = (size_t *)malloc((size_t)(N + 1) * sizeof(size_t));
    tz_len = (size_t *)malloc((size_t)(N + 1) * sizeof(size_t));
    tz_off[0] = 0; tz_len[0] = 0;
    for (j = 1; j <= N; j++) {
        tz_off[j] = total;
        tz_len[j] = tz_limbs(j);
        total += tz_len[j];
    }
    tz_data = (uint32_t *)calloc(total, sizeof(uint32_t));
    tz_data[tz_off[1]] = 1;
    for (k = 2; k <= N; k++) {
        uint32_t *p = tz_data + tz_off[k];
        const uint32_t *q = tz_data + tz_off[k - 1];
        uint64_t carry = 0;
        for (i = 0; i < tz_len[k]; i++) {
            uint64_t x = carry;
            if (i < tz_len[k - 1]) x += (uint64_t)(k - 1) * q[i];
            p[i] = (uint32_t)x;
            carry = x >> 32;
        }
    }
    for (k = 2; k <= N; k++) {
        for (j = k; j <= N; j++) {
            uint32_t *p = tz_data + tz_off[j];
            const uint32_t *q = tz_data + tz_off[j - 1];
            uint64_t c1 = (uint64_t)(j - k), c2 = (uint64_t)(j - k + 2);
            size_t lq = tz_len[j - 1];
            uint64_t carry = 0;
            for (i = 0; i < tz_len[j]; i++) {
                uint64_t x = c2 * p[i] + carry;
                if (i < lq) x += c1 * q[i];
                p[i] = (uint32_t)x;
                carry = x >> 32;
            }
        }
    }
    tz_cap = N;
}

seq_t a(idx_t n) {
    int W = tz_width();
    idx_t v, t, N;
    const uint32_t *p;
    size_t len;
    seq_t r = 0;

    if (n == 0) return 0;
    if (W < 300 && n > 300) return 0; /* value cannot fit in a fixed-width seq_t */

    if (n > tz_cap) {
        N = n;
        if (N < 2 * tz_cap) N = 2 * tz_cap;
        if (N < 32) N = 32;
        tz_build(N);
    }

    v = 0; t = n;
    while ((t & 1) == 0) { v++; t >>= 1; }
    v = 2 * n - 2 - v;

    p = tz_data + tz_off[n];
    len = tz_len[n];

    if (W < 300) {
        idx_t b = v + (idx_t)W;
        while (b > v) {
            idx_t bit;
            b--;
            bit = ((size_t)(b / 32) < len) ? ((p[b / 32] >> (b % 32)) & 1u) : 0;
            r = (seq_t)((r << 1) | (seq_t)bit);
        }
        return r;
    } else {
        size_t top = len;
        while (top > 0 && p[top - 1] == 0) top--;
        while (top > 0) {
            top--;
            r = (r << 32) | (seq_t)p[top];
        }
        return r >> v;
    }
}
