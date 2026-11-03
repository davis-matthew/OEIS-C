/*
 * A004601  Expansion of Pi in base 2 (or, binary expansion of Pi).
 * https://oeis.org/A004601
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(Pi * 2^(n-3)) mod 2; Pi computed in fixed-point
 *            binary by Machin: Pi = 16*arctan(1/5) - 4*arctan(1/239)
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 121 terms through n = 9250 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fx_divsmall(uint32_t *a, long long L, uint32_t d) {
    uint64_t r = 0;
    long long i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int fx_iszero(const uint32_t *a, long long L) {
    long long i;
    for (i = 0; i < L; i++) if (a[i]) return 0;
    return 1;
}

static void fx_mulsmall(uint32_t *a, long long L, uint32_t m) {
    uint64_t c = 0;
    long long i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)cur;
        c = cur >> 32;
    }
}

static uint32_t *P_pi = NULL;
static long long P_F = 0;   /* fractional bits held */
static long long P_L = 0;   /* limbs */

static void fx_add(uint32_t *a, const uint32_t *b, long long L) {
    uint64_t c = 0;
    long long i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)cur;
        c = cur >> 32;
    }
}

static void fx_sub(uint32_t *a, const uint32_t *b, long long L) {
    int64_t br = 0;
    long long i;
    for (i = 0; i < L; i++) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - br;
        if (cur < 0) { cur += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)cur;
    }
}

/* out = arctan(1/x) * 2^F */
static void fx_atan_inv(uint32_t *out, long long L, long long F, uint32_t x) {
    uint32_t *term = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)malloc((size_t)L * sizeof(uint32_t));
    uint32_t x2 = x * x;
    uint64_t k = 0;
    memset(out, 0, (size_t)L * sizeof(uint32_t));
    term[F / 32] = (uint32_t)1u << (F % 32);
    fx_divsmall(term, L, x);
    while (!fx_iszero(term, L)) {
        memcpy(tmp, term, (size_t)L * sizeof(uint32_t));
        fx_divsmall(tmp, L, (uint32_t)(2 * k + 1));
        if (k % 2 == 0) fx_add(out, tmp, L);
        else fx_sub(out, tmp, L);
        fx_divsmall(term, L, x2);
        k++;
    }
    free(term);
    free(tmp);
}

static void compute_pi(long long need) {
    long long F = ((need + 64) / 32 + 1) * 32;
    long long L = F / 32 + 2;
    uint32_t *a5 = (uint32_t *)malloc((size_t)L * sizeof(uint32_t));
    uint32_t *a239 = (uint32_t *)malloc((size_t)L * sizeof(uint32_t));
    fx_atan_inv(a5, L, F, 5);
    fx_atan_inv(a239, L, F, 239);
    fx_mulsmall(a5, L, 16);
    fx_mulsmall(a239, L, 4);
    fx_sub(a5, a239, L);
    free(a239);
    if (P_pi) free(P_pi);
    P_pi = a5;
    P_F = F;
    P_L = L;
}

seq_t a(idx_t n) {
    long long nn = (long long)n;
    long long p;
    if (n < 2) return 0;
    if (P_pi == NULL || nn + 64 > P_F) {
        long long need = nn * 2;
        if (need < 256) need = 256;
        compute_pi(need);
    }
    p = P_F + 3 - nn;  /* bit of floor(Pi * 2^(n-3)) at position 0 */
    return (seq_t)((P_pi[p / 32] >> (p % 32)) & 1u);
}
