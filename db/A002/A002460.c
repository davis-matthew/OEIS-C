/*
 * A002460  Nearest integer to exponential integral of n.
 * https://oeis.org/A002460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = round(Ei(n)), with Ei(n) = gamma + log(n) + Sum_{k>=1}
 *            n^k/(k*k!) and log(n) = e*log(2) + 2*atanh((n-2^e)/(n+2^e)),
 *            log(2) = 2*atanh(1/3); evaluated in exact multi-limb fixed-point
 *            arithmetic
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void ei_addto(uint32_t *acc, size_t L, const uint32_t *b, size_t bl)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < bl; i++) {
        uint64_t cur = (uint64_t)acc[i] + b[i] + carry;
        acc[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    for (; carry && i < L; i++) {
        uint64_t cur = (uint64_t)acc[i] + carry;
        acc[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void ei_div(uint32_t *x, size_t *xl, uint32_t d)
{
    uint64_t rem = 0;
    size_t i;
    for (i = *xl; i-- > 0;) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (*xl > 0 && x[*xl - 1] == 0) (*xl)--;
}

static void ei_mul(uint32_t *x, size_t *xl, size_t L, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *xl; i++) {
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    while (carry && *xl < L) {
        x[*xl] = (uint32_t)carry;
        carry >>= 32;
        (*xl)++;
    }
    while (*xl > 0 && x[*xl - 1] == 0) (*xl)--;
}

/* acc += mult * 2 * atanh(p/q), fixed point with FL fractional limbs */
static void ei_atanh(uint32_t *acc, size_t L, size_t FL, uint32_t mult,
                     uint32_t p, uint32_t q, uint32_t *t, uint32_t *w)
{
    size_t tl, wl;
    uint32_t j;
    if (p == 0 || mult == 0) return;
    memset(t, 0, L * sizeof(uint32_t));
    t[FL] = p;
    tl = FL + 1;
    ei_div(t, &tl, q);
    for (j = 0; tl > 0; j++) {
        memcpy(w, t, tl * sizeof(uint32_t));
        wl = tl;
        ei_div(w, &wl, 2 * j + 1);
        ei_mul(w, &wl, L, 2 * mult);
        ei_addto(acc, L, w, wl);
        ei_mul(t, &tl, L, p);
        ei_div(t, &tl, q);
        ei_mul(t, &tl, L, p);
        ei_div(t, &tl, q);
    }
}

seq_t a(idx_t n)
{
    static const char *gdig =
        "577215664901532860606512090082402431042159335939923598805767";
    uint64_t nn = (uint64_t)n;
    size_t FL = (size_t)((nn * 14427 / 10000 + 128) / 32 + 1);
    size_t IL = (size_t)((nn * 14427 / 10000 + 64) / 32 + 2);
    size_t L = FL + IL;
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t tl, ul, i;
    uint32_t k, e;
    uint64_t pw;
    seq_t r = 0;

    /* series Sum_{k>=1} n^k/(k*k!) */
    t[FL] = 1;
    tl = FL + 1;
    for (k = 1; tl > 0; k++) {
        ei_mul(t, &tl, L, (uint32_t)nn);
        ei_div(t, &tl, k);
        if (tl == 0) break;
        memcpy(u, t, tl * sizeof(uint32_t));
        ul = tl;
        ei_div(u, &ul, k);
        ei_addto(s, L, u, ul);
    }

    /* log(n) */
    e = 0;
    pw = 1;
    while (pw * 2 <= nn) { pw *= 2; e++; }
    ei_atanh(s, L, FL, e, 1, 3, t, u);
    ei_atanh(s, L, FL, 1, (uint32_t)(nn - pw), (uint32_t)(nn + pw), t, u);

    /* gamma */
    memset(t, 0, L * sizeof(uint32_t));
    {
        size_t len = strlen(gdig);
        for (i = len; i-- > 0;) {
            t[FL] += (uint32_t)(gdig[i] - '0');
            tl = FL + 1;
            ei_div(t, &tl, 10);
        }
        tl = FL + 1;
        while (tl > 0 && t[tl - 1] == 0) tl--;
        ei_addto(s, L, t, tl);
    }

    /* round to nearest */
    memset(t, 0, L * sizeof(uint32_t));
    t[FL - 1] = 0x80000000u;
    ei_addto(s, L, t, FL);

    for (i = L; i-- > FL;) {
        r *= 65536;
        r *= 65536;
        r += (seq_t)s[i];
    }
    free(s);
    free(t);
    free(u);
    return r;
}
