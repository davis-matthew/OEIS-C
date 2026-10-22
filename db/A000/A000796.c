/*
 * A000796  Decimal expansion of Pi (or digits of Pi).
 * https://oeis.org/A000796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, nice, core, easy
 * Formula:   Pi = 16*arctan(1/5) - 4*arctan(1/239) (Machin, 1706),
 *            arctan(1/m) = Sum_{k>=0} (-1)^k/((2k+1)*m^(2k+1)); a(n) = n-th
 *            digit of Pi, computed in fixed-point base-10^9 arithmetic
 * Range:     u8 n <= 9880, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PI_BASE 1000000000ULL

static uint32_t *pi_limbs = NULL;
static idx_t pi_ndig = 0;

static void pi_div(uint32_t *x, size_t s, size_t L, uint64_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = s; i < L; i++) {
        uint64_t c = r * PI_BASE + x[i];
        x[i] = (uint32_t)(c / d);
        r = c % d;
    }
}

static void pi_addsub(uint32_t *res, const uint32_t *t, size_t s, size_t L, int add) {
    int64_t carry = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        int64_t v = (int64_t)res[i] + carry;
        if (i >= s) v += add ? (int64_t)t[i] : -(int64_t)t[i];
        if (v < 0) { v += (int64_t)PI_BASE; carry = -1; }
        else if (v >= (int64_t)PI_BASE) { v -= (int64_t)PI_BASE; carry = 1; }
        else carry = 0;
        res[i] = (uint32_t)v;
        if (i < s && carry == 0) break;
    }
}

static void pi_atan(uint32_t *res, uint32_t *p, uint32_t *t, size_t L,
                    uint32_t coef, uint64_t m, int positive) {
    size_t s = 0;
    uint64_t k;
    memset(p, 0, L * sizeof(uint32_t));
    p[0] = coef;
    pi_div(p, 0, L, m);
    for (k = 0;; k++) {
        while (s < L && p[s] == 0) s++;
        if (s == L) break;
        memcpy(t + s, p + s, (L - s) * sizeof(uint32_t));
        pi_div(t, s, L, 2 * k + 1);
        pi_addsub(res, t, s, L, ((k & 1) == 0) == (positive != 0));
        pi_div(p, s, L, m * m);
    }
}

static void pi_compute(idx_t ndig) {
    size_t L = (size_t)(ndig / 9) + 6;
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc(L * sizeof(uint32_t));
    pi_atan(res, p, t, L, 16, 5, 1);
    pi_atan(res, p, t, L, 4, 239, 0);
    free(p);
    free(t);
    free(pi_limbs);
    pi_limbs = res;
    pi_ndig = ndig;
}

seq_t a(idx_t n) {
    idx_t f, li, j;
    uint32_t v;
    if (n < 1) return 0;
    if (n > pi_ndig) {
        idx_t want = n + 32;
        if (want < 2 * pi_ndig) want = 2 * pi_ndig;
        if (want < 1000) want = 1000;
        pi_compute(want);
    }
    if (n == 1) return (seq_t)pi_limbs[0];
    f = n - 1;
    li = (f - 1) / 9 + 1;
    j = (f - 1) % 9;
    v = pi_limbs[li];
    for (; j < 8; j++) v /= 10;
    return (seq_t)(v % 10);
}
