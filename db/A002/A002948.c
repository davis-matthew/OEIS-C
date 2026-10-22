/*
 * A002948  Continued fraction for cube root of 5.
 * https://oeis.org/A002948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) is the n-th partial quotient of the continued fraction of
 *            5^(1/3). Computed exactly with Lagrange's method. Keep an
 *            integer cubic P(x) = c3 x^3 + c2 x^2 + c1 x + c0 whose only real
 *            root is the current complete quotient x_n. Start with P(x) = x^3
 *            - 5. a(n) = floor(x_n) is the largest t with P...
 * Range:     u8 n <= 17, u16 n <= 9641, u32 n <= 9641, u64 n <= 9641,
 *            u128 n <= 9641, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *a2948_c[4];
static uint32_t *a2948_tmp;
static size_t a2948_W = 0;
static uint32_t *a2948_terms = NULL;
static size_t a2948_len = 0, a2948_cap = 0;

/* dst = x*t + y, two's complement over W limbs */
static void a2948_madd(uint32_t *dst, const uint32_t *x, uint32_t t, const uint32_t *y)
{
    uint64_t mc = 0, ac = 0;
    size_t i;
    for (i = 0; i < a2948_W; i++) {
        uint64_t p = (uint64_t)x[i] * t + mc;
        uint64_t s;
        mc = p >> 32;
        s = (uint64_t)(uint32_t)p + y[i] + ac;
        dst[i] = (uint32_t)s;
        ac = s >> 32;
    }
}

static void a2948_neg(uint32_t *x)
{
    uint64_t c = 1;
    size_t i;
    for (i = 0; i < a2948_W; i++) {
        uint64_t s = (uint64_t)(uint32_t)~x[i] + c;
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static int a2948_isneg(const uint32_t *x)
{
    return (int)(x[a2948_W - 1] >> 31);
}

/* is P(t) < 0 ? */
static int a2948_evalneg(uint32_t t)
{
    memcpy(a2948_tmp, a2948_c[3], a2948_W * sizeof(uint32_t));
    a2948_madd(a2948_tmp, a2948_tmp, t, a2948_c[2]);
    a2948_madd(a2948_tmp, a2948_tmp, t, a2948_c[1]);
    a2948_madd(a2948_tmp, a2948_tmp, t, a2948_c[0]);
    return a2948_isneg(a2948_tmp);
}

static void a2948_resize(size_t nw)
{
    int k;
    size_t i;
    for (k = 0; k < 4; k++) {
        uint32_t ext;
        a2948_c[k] = (uint32_t *)realloc(a2948_c[k], nw * sizeof(uint32_t));
        ext = (a2948_W > 0 && (a2948_c[k][a2948_W - 1] >> 31)) ? 0xFFFFFFFFu : 0u;
        for (i = a2948_W; i < nw; i++) a2948_c[k][i] = ext;
    }
    a2948_tmp = (uint32_t *)realloc(a2948_tmp, nw * sizeof(uint32_t));
    a2948_W = nw;
}

static void a2948_ensure_room(void)
{
    int k, need = 0;
    size_t i;
    for (k = 0; k < 4 && !need; k++) {
        uint32_t *x = a2948_c[k];
        uint32_t ext = (x[a2948_W - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        for (i = a2948_W - 7; i < a2948_W; i++)
            if (x[i] != ext) { need = 1; break; }
        if (!need && ((x[a2948_W - 8] >> 31) != (ext & 1u))) need = 1;
    }
    if (need) a2948_resize(a2948_W + a2948_W / 2 + 16);
}

static void a2948_init(void)
{
    int k;
    a2948_resize(16);
    for (k = 0; k < 4; k++) memset(a2948_c[k], 0, a2948_W * sizeof(uint32_t));
    a2948_c[3][0] = 1;
    a2948_c[0][0] = 5;
    a2948_neg(a2948_c[0]);
}

static uint32_t a2948_step(void)
{
    uint64_t lo = 1, hi = 2;
    uint32_t t;
    int i, j;
    uint32_t *r0, *r1;

    a2948_ensure_room();
    while (hi < 0xFFFFFFFFull && a2948_evalneg((uint32_t)hi)) {
        lo = hi;
        hi *= 2;
        if (hi > 0xFFFFFFFFull) hi = 0xFFFFFFFFull;
    }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a2948_evalneg((uint32_t)mid)) lo = mid; else hi = mid;
    }
    t = (uint32_t)lo;

    /* Taylor shift: P(x) -> P(x + t) */
    for (i = 0; i < 3; i++)
        for (j = 2; j >= i; j--)
            a2948_madd(a2948_c[j], a2948_c[j + 1], t, a2948_c[j]);

    /* reverse coefficients: x^3 * P(1/x) */
    r0 = a2948_c[0]; r1 = a2948_c[1];
    a2948_c[0] = a2948_c[3]; a2948_c[3] = r0;
    a2948_c[1] = a2948_c[2]; a2948_c[2] = r1;

    if (a2948_isneg(a2948_c[3]))
        for (i = 0; i < 4; i++) a2948_neg(a2948_c[i]);
    return t;
}

seq_t a(idx_t n)
{
    if (a2948_W == 0) a2948_init();
    if ((size_t)n >= a2948_cap) {
        size_t nc = (size_t)n + 1 + (size_t)n / 2 + 16;
        a2948_terms = (uint32_t *)realloc(a2948_terms, nc * sizeof(uint32_t));
        a2948_cap = nc;
    }
    while (a2948_len <= (size_t)n) {
        a2948_terms[a2948_len] = a2948_step();
        a2948_len++;
    }
    return (seq_t)a2948_terms[n];
}
