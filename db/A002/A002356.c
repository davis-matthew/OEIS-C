/*
 * A002356  Numerators of convergents to cube root of 4.
 * https://oeis.org/A002356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(-2) = 0, where c(n) is
 *            the n-th partial quotient of 4^(1/3). The c(n) are computed
 *            exactly with Lagrange's method: keep a cubic with integer
 *            coefficients whose only real root is the current complete
 *            quotient; c = floor(root) is found by exact sign tests,...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *a2356_C[4];
static uint32_t *a2356_T = NULL;
static size_t a2356_L = 0;
static uint32_t *a2356_pq = NULL;
static size_t a2356_npq = 0, a2356_cap = 0;

static size_t a2356_sig(const uint32_t *x) {
    uint32_t s = (x[a2356_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
    size_t i = a2356_L;
    while (i > 0 && x[i - 1] == s) i--;
    return i;
}

static void a2356_grow(size_t nl) {
    int j;
    size_t i;
    for (j = 0; j < 4; j++) {
        uint32_t s = (a2356_C[j][a2356_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        a2356_C[j] = (uint32_t *)realloc(a2356_C[j], nl * sizeof(uint32_t));
        for (i = a2356_L; i < nl; i++) a2356_C[j][i] = s;
    }
    a2356_T = (uint32_t *)realloc(a2356_T, nl * sizeof(uint32_t));
    a2356_L = nl;
}

static void a2356_init(void) {
    int j;
    size_t i;
    a2356_L = 8;
    for (j = 0; j < 4; j++) a2356_C[j] = (uint32_t *)calloc(a2356_L, sizeof(uint32_t));
    a2356_T = (uint32_t *)calloc(a2356_L, sizeof(uint32_t));
    /* x^3 - 4 */
    a2356_C[3][0] = 1;
    for (i = 0; i < a2356_L; i++) a2356_C[0][i] = 0xFFFFFFFFu;
    a2356_C[0][0] = 0xFFFFFFFCu;
}

/* true if P(k) has the sign of the leading coefficient, i.e. k > root */
static int a2356_above(uint32_t k) {
    int j;
    size_t i;
    memcpy(a2356_T, a2356_C[3], a2356_L * sizeof(uint32_t));
    for (j = 2; j >= 0; j--) {
        uint64_t carry = 0;
        const uint32_t *c = a2356_C[j];
        for (i = 0; i < a2356_L; i++) {
            uint64_t t = (uint64_t)a2356_T[i] * k + c[i] + carry;
            a2356_T[i] = (uint32_t)t;
            carry = t >> 32;
        }
    }
    return (a2356_T[a2356_L - 1] >> 31) == (a2356_C[3][a2356_L - 1] >> 31);
}

static void a2356_addmul(uint32_t *dst, const uint32_t *src, uint32_t k) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a2356_L; i++) {
        uint64_t t = (uint64_t)src[i] * k + dst[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a2356_step(void) {
    size_t m = 0, s;
    int i, j;
    uint32_t lo, hi, k;
    uint32_t *tmp;
    for (j = 0; j < 4; j++) {
        s = a2356_sig(a2356_C[j]);
        if (s > m) m = s;
    }
    if (m + 7 > a2356_L) a2356_grow(2 * a2356_L > m + 16 ? 2 * a2356_L : m + 16);
    /* root > 1 always; gallop then bisect */
    lo = 1;
    hi = 2;
    while (!a2356_above(hi) && hi < 0x80000000u) {
        lo = hi;
        hi *= 2;
    }
    while (hi - lo > 1) {
        uint32_t mid = lo + (hi - lo) / 2;
        if (a2356_above(mid)) hi = mid;
        else lo = mid;
    }
    k = lo;
    /* P(x) -> P(k + x) */
    for (i = 0; i < 3; i++)
        for (j = 2; j >= i; j--)
            a2356_addmul(a2356_C[j], a2356_C[j + 1], k);
    /* reverse: y^3 * P(k + 1/y) */
    tmp = a2356_C[0]; a2356_C[0] = a2356_C[3]; a2356_C[3] = tmp;
    tmp = a2356_C[1]; a2356_C[1] = a2356_C[2]; a2356_C[2] = tmp;
    if (a2356_npq == a2356_cap) {
        a2356_cap = a2356_cap ? 2 * a2356_cap : 1024;
        a2356_pq = (uint32_t *)realloc(a2356_pq, a2356_cap * sizeof(uint32_t));
    }
    a2356_pq[a2356_npq++] = k;
}

seq_t a(idx_t n) {
    seq_t hm2 = 0, hm1 = 1;
    idx_t i;
    if (a2356_L == 0) a2356_init();
    while ((idx_t)a2356_npq <= n) a2356_step();
    for (i = 0; i <= n; i++) {
        seq_t h = (seq_t)a2356_pq[i] * hm1 + hm2;
        hm2 = hm1;
        hm1 = h;
    }
    return hm1;
}
