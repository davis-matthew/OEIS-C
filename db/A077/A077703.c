/*
 * A077703  a(n) = A077702(n+1)/A077702(n).
 * https://oeis.org/A077703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A077702(n+1)/A077702(n), found by search: A077702(1) = 5,
 *            and a(n) is the smallest odd k >= 3 such that A077702(n)*k
 *            contains each digit of A077702(n) at least as many times; then
 *            A077702(n+1) = A077702(n)*a(n). A077702 is kept internally as a
 *            base-10^4 limb array, so the huge intermediate...
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A077703_BASE 10000u

static uint64_t A077703_TL[10000];
static uint64_t A077703_TH[10000];
static bool A077703_tinit = false;
static uint64_t A077703_G = 0;

static uint32_t *A077703_P = NULL;
static size_t A077703_LP = 0;
static uint64_t *A077703_ks = NULL;
static idx_t A077703_nks = 0;
static idx_t A077703_capks = 0;

static void A077703_init_tables(void)
{
    for (uint32_t v = 0; v < 10000u; ++v) {
        uint64_t lo = 0, hi = 0;
        uint32_t x = v;
        for (int j = 0; j < 4; ++j) {
            uint32_t d = x % 10u;
            x /= 10u;
            if (d < 5) lo += (uint64_t)1 << (12 * d);
            else hi += (uint64_t)1 << (12 * (d - 5));
        }
        A077703_TL[v] = lo;
        A077703_TH[v] = hi;
    }
    A077703_G = 0;
    for (int f = 0; f < 5; ++f) A077703_G |= (uint64_t)0x800 << (12 * f);
    A077703_tinit = true;
}

static int A077703_ndig4(uint32_t t)
{
    return t >= 1000u ? 4 : t >= 100u ? 3 : t >= 10u ? 2 : 1;
}

static void A077703_counts(const uint32_t *c, size_t len, uint64_t *lo, uint64_t *hi)
{
    uint64_t sl = 0, sh = 0;
    for (size_t i = 0; i < len; ++i) {
        sl += A077703_TL[c[i]];
        sh += A077703_TH[c[i]];
    }
    sl -= (uint64_t)(4 - A077703_ndig4(c[len - 1]));
    *lo = sl;
    *hi = sh;
}

static bool A077703_ok(uint64_t slo, uint64_t shi, uint64_t RL, uint64_t RH)
{
    uint64_t G = A077703_G;
    return ((((slo | G) - RL) & G) == G) && ((((shi | G) - RH) & G) == G);
}

static void A077703_step(void)
{
    size_t LP = A077703_LP;
    const uint32_t *P = A077703_P;
    uint64_t RL, RH;
    A077703_counts(P, LP, &RL, &RH);

    size_t cap = LP + 12;
    uint32_t *C = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *D = (uint32_t *)calloc(cap, sizeof(uint32_t));

    /* D = 2P, C = 3P */
    uint32_t cd = 0, cc = 0;
    for (size_t i = 0; i < LP; ++i) {
        uint32_t v = 2u * P[i] + cd;
        D[i] = v % A077703_BASE; cd = v / A077703_BASE;
        uint32_t w = 3u * P[i] + cc;
        C[i] = w % A077703_BASE; cc = w / A077703_BASE;
    }
    if (cd) D[LP] = cd;
    size_t lenC = LP;
    if (cc) C[lenC++] = cc;

    uint64_t k = 3;
    uint64_t slo, shi;
    A077703_counts(C, lenC, &slo, &shi);
    if (!A077703_ok(slo, shi, RL, RH)) {
        for (;;) {
            k += 2;
            uint32_t carry = 0;
            slo = 0; shi = 0;
            for (size_t i = 0; i < lenC; ++i) {
                uint32_t v = C[i] + D[i] + carry;
                carry = (v >= A077703_BASE);
                if (carry) v -= A077703_BASE;
                C[i] = v;
                slo += A077703_TL[v];
                shi += A077703_TH[v];
            }
            if (carry) {
                if (lenC + 1 >= cap) {
                    size_t nc = cap * 2;
                    C = (uint32_t *)realloc(C, nc * sizeof(uint32_t));
                    D = (uint32_t *)realloc(D, nc * sizeof(uint32_t));
                    for (size_t i = cap; i < nc; ++i) { C[i] = 0; D[i] = 0; }
                    cap = nc;
                }
                C[lenC++] = 1;
                slo += A077703_TL[1];
                shi += A077703_TH[1];
            }
            slo -= (uint64_t)(4 - A077703_ndig4(C[lenC - 1]));
            if (A077703_ok(slo, shi, RL, RH)) break;
        }
    }

    free(D);
    free(A077703_P);
    A077703_P = C;
    A077703_LP = lenC;

    if (A077703_nks >= A077703_capks) {
        idx_t nc = A077703_capks ? A077703_capks * 2 : 32;
        A077703_ks = (uint64_t *)realloc(A077703_ks, (size_t)nc * sizeof(uint64_t));
        A077703_capks = nc;
    }
    A077703_ks[A077703_nks++] = k;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A077703_tinit) A077703_init_tables();
    if (A077703_P == NULL) {
        A077703_P = (uint32_t *)calloc(4, sizeof(uint32_t));
        A077703_P[0] = 5;
        A077703_LP = 1;
    }
    while (A077703_nks < n) A077703_step();
    return (seq_t)A077703_ks[n - 1];
}
