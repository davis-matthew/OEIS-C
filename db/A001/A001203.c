/*
 * A001203  Simple continued fraction expansion of Pi.
 * https://oeis.org/A001203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, cofr
 * Formula:   no closed form. a(n) is the n-th partial quotient of the simple
 *            continued fraction of Pi. The code computes Pi with big-integer
 *            arithmetic using Machin's formula, Pi = 16*arctan(1/5) -
 *            4*arctan(1/239), and tracks the rounding error so it has exact
 *            rational bounds lo < Pi < hi. It then runs Euclid...
 * Range:     u8 n <= 3, u16 n <= 19999, u32 n <= 19999, u64 n <= 19999,
 *            u128 n <= 19999, big any n
 * Validated: standard, pass, 114 terms through n = 9856 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void pcf_addmul(uint32_t *acc, const uint32_t *x, size_t L, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)acc[i] + (uint64_t)x[i] * m + carry;
        acc[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void pcf_addsmall(uint32_t *x, size_t L, uint64_t v)
{
    size_t i;
    for (i = 0; i < L && v; i++) {
        uint64_t cur = (uint64_t)x[i] + (v & 0xFFFFFFFFu);
        x[i] = (uint32_t)cur;
        v = (v >> 32) + (cur >> 32);
    }
}

static void pcf_divs(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t rem = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static int pcf_zero(const uint32_t *x, size_t L)
{
    size_t i;
    for (i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

/* adds mult*arctan(1/x)*2^M into S (positive terms) and T (negative terms) */
static uint64_t pcf_atan(uint32_t *S, uint32_t *T, size_t L, uint32_t x,
                         uint32_t mult, int neg, uint32_t *t, uint32_t *tmp)
{
    uint64_t k = 0;
    memset(t, 0, L * sizeof(uint32_t));
    t[L - 1] = 1;
    pcf_divs(t, L, x);
    while (!pcf_zero(t, L)) {
        memcpy(tmp, t, L * sizeof(uint32_t));
        pcf_divs(tmp, L, (uint32_t)(2 * k + 1));
        pcf_addmul((((int)(k & 1)) ^ neg) ? T : S, tmp, L, mult);
        pcf_divs(t, L, x * x);
        k++;
    }
    return k;
}

static size_t pcf_bits(const uint32_t *x, size_t lx)
{
    size_t b = 0;
    uint32_t top;
    if (!lx) return 0;
    top = x[lx - 1];
    while (top) { b++; top >>= 1; }
    return 32 * (lx - 1) + b;
}

static int pcf_cmp(const uint32_t *a, size_t la, const uint32_t *b, size_t lb)
{
    size_t i;
    if (la != lb) return la > lb ? 1 : -1;
    i = la;
    while (i--) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static uint64_t pcf_limb(const uint32_t *x, size_t lx, size_t i)
{
    return i < lx ? x[i] : 0;
}

static uint64_t pcf_ext(const uint32_t *x, size_t lx, size_t sh)
{
    size_t idx = sh / 32, off = sh % 32;
    uint64_t v = pcf_limb(x, lx, idx) | (pcf_limb(x, lx, idx + 1) << 32);
    if (off)
        v = (v >> off) | (pcf_limb(x, lx, idx + 2) << (64 - off));
    return v;
}

static size_t pcf_norm(const uint32_t *x, size_t len)
{
    while (len && x[len - 1] == 0) len--;
    return len;
}

/* a -= (b*m) << k, requires result >= 0 */
static void pcf_submul(uint32_t *a, size_t *la, const uint32_t *b, size_t lb,
                       uint32_t m, size_t k)
{
    size_t ws = k / 32, bs = k % 32, i, idx;
    uint64_t carry = 0, borrow = 0;
    uint32_t prev = 0;
    size_t top = *la;
    for (i = 0; i <= lb + 1; i++) {
        uint32_t p, s;
        uint64_t d;
        if (i < lb) {
            uint64_t c = (uint64_t)b[i] * m + carry;
            p = (uint32_t)c;
            carry = c >> 32;
        } else if (i == lb) {
            p = (uint32_t)carry;
            carry = 0;
        } else {
            p = 0;
        }
        s = bs ? (uint32_t)((p << bs) | (uint32_t)((uint64_t)prev >> (32 - bs))) : p;
        prev = p;
        idx = ws + i;
        d = (uint64_t)a[idx] - s - borrow;
        a[idx] = (uint32_t)d;
        borrow = (d >> 63) & 1;
    }
    idx = ws + lb + 2;
    while (borrow) {
        uint64_t d = (uint64_t)a[idx] - borrow;
        a[idx] = (uint32_t)d;
        borrow = (d >> 63) & 1;
        idx++;
    }
    if (ws + lb + 2 > top) top = ws + lb + 2;
    *la = pcf_norm(a, top);
}

/* a <- a mod b, returns floor(a/b) */
static uint64_t pcf_step(uint32_t *a, size_t *la, const uint32_t *b, size_t lb)
{
    uint64_t q = 0;
    while (pcf_cmp(a, *la, b, lb) >= 0) {
        size_t ba = pcf_bits(a, *la), bb = pcf_bits(b, lb);
        size_t d = ba - bb;
        if (d <= 30) {
            size_t sh = bb > 32 ? bb - 32 : 0;
            uint64_t A = pcf_ext(a, *la, sh), B = pcf_ext(b, lb, sh);
            uint64_t qe = A / (B + 1);
            if (!qe) qe = 1;
            pcf_submul(a, la, b, lb, (uint32_t)qe, 0);
            q += qe;
        } else {
            pcf_submul(a, la, b, lb, 1, d - 1);
            q += (uint64_t)1 << (d - 1 < 63 ? d - 1 : 63);
        }
    }
    return q;
}

static void pcf_subn(uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t d = (uint64_t)a[i] - b[i] - borrow;
        a[i] = (uint32_t)d;
        borrow = (d >> 63) & 1;
    }
}

static void pcf_subsmall(uint32_t *x, size_t L, uint64_t v)
{
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < L && (v || borrow); i++) {
        uint64_t d = (uint64_t)x[i] - (v & 0xFFFFFFFFu) - borrow;
        x[i] = (uint32_t)d;
        borrow = (d >> 63) & 1;
        v >>= 32;
    }
}

static size_t pcf_run(size_t L, uint64_t **out)
{
    size_t cap = L + 4, i;
    uint32_t *S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *a1, *b1, *a2, *b2, *sw;
    size_t la1, lb1, la2, lb2, lsw;
    uint64_t c5, c239, E;
    size_t cnt = 0, tcap = 1024;
    uint64_t *terms = (uint64_t *)malloc(tcap * sizeof(uint64_t));

    c5 = pcf_atan(S, T, L, 5, 16, 0, t, tmp);
    c239 = pcf_atan(S, T, L, 239, 4, 1, t, tmp);
    pcf_subn(S, T, L);
    E = 16 * (2 * c5 + 1) + 4 * (2 * c239 + 1) + 1;

    /* a1 = lo = X - E, a2 = hi = X + E, b1 = b2 = 2^M */
    a1 = t; a2 = tmp; b1 = T; b2 = S;
    memcpy(a1, S, cap * sizeof(uint32_t));
    memcpy(a2, S, cap * sizeof(uint32_t));
    pcf_subsmall(a1, cap, E);
    pcf_addsmall(a2, cap, E);
    memset(b1, 0, cap * sizeof(uint32_t));
    memset(b2, 0, cap * sizeof(uint32_t));
    b1[L - 1] = 1;
    b2[L - 1] = 1;
    la1 = pcf_norm(a1, cap); la2 = pcf_norm(a2, cap);
    lb1 = pcf_norm(b1, cap); lb2 = pcf_norm(b2, cap);

    for (;;) {
        uint64_t q1, q2;
        if (lb1 == 0 || lb2 == 0) break;
        q1 = pcf_step(a1, &la1, b1, lb1);
        q2 = pcf_step(a2, &la2, b2, lb2);
        if (q1 != q2) break;
        if (cnt == tcap) {
            tcap *= 2;
            terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
        }
        terms[cnt++] = q1;
        sw = a1; a1 = b1; b1 = sw; lsw = la1; la1 = lb1; lb1 = lsw;
        sw = a2; a2 = b2; b2 = sw; lsw = la2; la2 = lb2; lb2 = lsw;
    }
    for (i = 0; i < 0; i++) {}
    free(S); free(T); free(t); free(tmp);
    *out = terms;
    return cnt;
}

static uint64_t *pcf_terms = NULL;
static size_t pcf_cnt = 0;

static void pcf_compute(size_t N, idx_t need)
{
    size_t L = (4 * N + 256) / 32 + 2;
    for (;;) {
        uint64_t *terms;
        size_t cnt = pcf_run(L, &terms);
        if (cnt > pcf_cnt) {
            free(pcf_terms);
            pcf_terms = terms;
            pcf_cnt = cnt;
        } else {
            free(terms);
        }
        if ((idx_t)pcf_cnt > need) break;
        L += L / 2;
    }
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)pcf_cnt) {
        size_t N = (size_t)n + 1;
        if (N < 2 * pcf_cnt) N = 2 * pcf_cnt;
        pcf_compute(N, n);
    }
    return (seq_t)pcf_terms[n];
}
