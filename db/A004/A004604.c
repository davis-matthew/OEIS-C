/*
 * A004604  Expansion of Pi in base 5.
 * https://oeis.org/A004604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = n-th base-5 digit of Pi, computed via Machin's formula Pi
 *            = 16*arctan(1/5) - 4*arctan(1/239) in fixed-point base-5^12
 *            limbs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fx_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/fx_zero.h"

#define LB 244140625ULL /* 5^12 */
#define LD 12

static unsigned char *pi_dig = NULL;
static idx_t pi_ndig = 0;

static void fx_div(const uint32_t *x, uint32_t *out, int len, uint64_t d) {
    uint64_t r = 0;
    for (int i = 0; i < len; i++) {
        uint64_t cur = r * LB + x[i];
        out[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void fx_add(uint32_t *x, const uint32_t *y, int len) {
    uint64_t c = 0;
    for (int i = len - 1; i >= 0; i--) {
        uint64_t s = (uint64_t)x[i] + y[i] + c;
        if (s >= LB) { x[i] = (uint32_t)(s - LB); c = 1; }
        else { x[i] = (uint32_t)s; c = 0; }
    }
}

static void fx_sub(uint32_t *x, const uint32_t *y, int len) {
    int64_t b = 0;
    for (int i = len - 1; i >= 0; i--) {
        int64_t s = (int64_t)x[i] - (int64_t)y[i] - b;
        if (s < 0) { x[i] = (uint32_t)(s + (int64_t)LB); b = 1; }
        else { x[i] = (uint32_t)s; b = 0; }
    }
}

static void fx_mul(uint32_t *x, int len, uint64_t m) {
    uint64_t c = 0;
    for (int i = len - 1; i >= 0; i--) {
        uint64_t s = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)(s % LB);
        c = s / LB;
    }
}

static void fx_atan(uint32_t *res, int len, uint64_t m) {
    uint32_t *term = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    memset(res, 0, (size_t)len * sizeof(uint32_t));
    term[0] = 1;
    fx_div(term, term, len, m);
    uint64_t k = 0;
    while (!fx_zero(term, len)) {
        fx_div(term, tmp, len, 2 * k + 1);
        if (k % 2 == 0) fx_add(res, tmp, len);
        else fx_sub(res, tmp, len);
        fx_div(term, term, len, m * m);
        k++;
    }
    free(term);
    free(tmp);
}

static void compute_pi(idx_t need) {
    int L = (int)(need / LD) + 4;   /* fractional limbs incl. guard */
    int len = L + 1;                /* plus integer limb */
    uint32_t *p = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc((size_t)len, sizeof(uint32_t));
    fx_atan(p, len, 5);
    fx_mul(p, len, 16);
    fx_atan(q, len, 239);
    fx_mul(q, len, 4);
    fx_sub(p, q, len);

    idx_t total = 1 + (idx_t)(L - 2) * LD;
    unsigned char *d = (unsigned char *)malloc((size_t)total);
    d[0] = (unsigned char)p[0];
    idx_t pos = 1;
    for (int i = 1; i <= L - 2; i++) {
        uint32_t v = p[i];
        for (int j = LD - 1; j >= 0; j--) {
            d[pos + (idx_t)j] = (unsigned char)(v % 5);
            v /= 5;
        }
        pos += LD;
    }
    free(p);
    free(q);
    free(pi_dig);
    pi_dig = d;
    pi_ndig = total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > pi_ndig) {
        idx_t need = n;
        if (need < 2 * pi_ndig) need = 2 * pi_ndig;
        if (need < 256) need = 256;
        compute_pi(need);
    }
    return (seq_t)pi_dig[n - 1];
}
