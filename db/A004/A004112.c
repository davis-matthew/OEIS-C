/*
 * A004112  Numbers k where |cos(k)| (or |cosec(k)| or |cot(k)|) decreases
 *          monotonically to 0; also numbers k where |tan(k)| (or |sec(k)|, or
 *          |sin(k)|) increases.
 * https://oeis.org/A004112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 153, u32 n <= 166, u64 n <= 235,
 *            u128 n <= 314, big any n
 * Validated: standard, pass, 61 terms through n = 925 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A004112: the k at which |cos k| reaches a new minimum (k = 0, 1, 2, ...).
 *
 * Let f(k) be the signed distance from k to the nearest odd multiple of pi/2.
 * After a record k, the next one is k + q for the least q >= 1 whose residue
 * delta(q) = q - p pi (p the nearest integer) has the sign opposite to f and
 * |delta| < 2|f|: then |f + delta| < |f|. That least q is a best one-sided
 * approximation, i.e. a convergent or intermediate denominator of pi: with
 * x_-1 = pi, x_0 = 1, a_i = floor(x_(i-1)/x_i), x_(i+1) = x_(i-1) - a_i x_i
 * and q_-1 = 0, q_0 = 1, q_(i+1) = q_(i-1) + a_i q_i, the candidates in
 * increasing order are q_0 = 1 (delta = 1) and q_(i-1) + j q_i, j = 1..a_i,
 * with delta = (-1)^(i-1) (x_(i-1) - j x_i).
 *
 * The x_i are fixed-point numbers with 2304 fraction bits (pi from Machin's
 * formula), ample for terms up to 10^300; the terms themselves are exact
 * multi-limb integers, returned modulo the width for the fixed widths. (The
 * original scanned k in long double and was both slow and inexact.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FL 72                             /* fraction limbs (32 bits each) */
#define L (FL + 2)                        /* plus an integer part */
#define QL 40                             /* limbs of a term (1280 bits) */

typedef struct { uint32_t d[L]; } fx;     /* two's complement, d[0] lowest */
typedef struct { uint32_t d[QL]; } big;   /* unsigned */

static void fx_add(fx *r, const fx *a, const fx *b)
{
    uint64_t c = 0;
    for (int i = 0; i < L; ++i) { c += (uint64_t)a->d[i] + b->d[i]; r->d[i] = (uint32_t)c; c >>= 32; }
}
static void fx_neg(fx *r, const fx *a)
{
    uint64_t c = 1;
    for (int i = 0; i < L; ++i) { c += (uint64_t)(uint32_t)~a->d[i]; r->d[i] = (uint32_t)c; c >>= 32; }
}
static void fx_sub(fx *r, const fx *a, const fx *b) { fx t; fx_neg(&t, b); fx_add(r, a, &t); }
static int fx_neg_p(const fx *a) { return (int)(a->d[L - 1] >> 31); }
static void fx_abs(fx *r, const fx *a) { if (fx_neg_p(a)) fx_neg(r, a); else *r = *a; }
static int fx_cmp(const fx *a, const fx *b)          /* both non-negative */
{
    for (int i = L - 1; i >= 0; --i) if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}
static void fx_mul_small(fx *r, const fx *a, uint32_t m)  /* a >= 0 */
{
    uint64_t c = 0;
    for (int i = 0; i < L; ++i) { c += (uint64_t)a->d[i] * m; r->d[i] = (uint32_t)c; c >>= 32; }
}
static void fx_div_small(fx *r, const fx *a, uint32_t m)  /* a >= 0 */
{
    uint64_t rem = 0;
    for (int i = L - 1; i >= 0; --i) { uint64_t cur = (rem << 32) | a->d[i]; r->d[i] = (uint32_t)(cur / m); rem = cur % m; }
}
static double fx_dbl(const fx *a)          /* a >= 0, roughly */
{
    double v = 0;
    for (int i = L - 1; i >= L - 6; --i) v = v * 4294967296.0 + a->d[i];
    return v * pow(2.0, -32.0 * (FL - (L - 6)));
}

static void atan_inv(fx *r, uint32_t x)    /* atan(1/x) */
{
    fx p, t;
    memset(&p, 0, sizeof p);
    p.d[FL] = 1;
    fx_div_small(&p, &p, x);               /* 1/x */
    memset(r, 0, sizeof *r);
    for (uint32_t k = 0;; ++k) {
        fx_div_small(&t, &p, 2 * k + 1);
        int zero = 1;
        for (int i = 0; i < L; ++i) if (t.d[i]) { zero = 0; break; }
        if (zero) break;
        if (k & 1) fx_sub(r, r, &t); else fx_add(r, r, &t);
        fx_div_small(&p, &p, x * x);
    }
}

static void big_addmul(big *r, const big *a, const big *b, uint32_t j)   /* r = a + j b */
{
    uint64_t c = 0;
    for (int i = 0; i < QL; ++i) { c += (uint64_t)a->d[i] + (uint64_t)b->d[i] * j; r->d[i] = (uint32_t)c; c >>= 32; }
}

typedef struct { big q; fx delta; } cand;

static cand *cands = NULL;
static size_t ncand = 0, capc = 0;
static fx X0, X1;                          /* x_(i-1), x_i */
static big Q0, Q1;                         /* q_(i-1), q_i */
static int level = 0;                      /* i */

static void add_cand(const big *q, const fx *d)
{
    if (ncand == capc) {
        capc = capc ? 2 * capc : 1024;
        cand *t = (cand *)realloc(cands, capc * sizeof *t);
        if (!t) abort();
        cands = t;
    }
    cands[ncand].q = *q;
    cands[ncand].delta = *d;
    ++ncand;
}

static int more_cands(void)                /* the semiconvergents of one more level */
{
    if (Q1.d[QL - 2]) return 0;            /* past the precision kept */
    double est = fx_dbl(&X0) / fx_dbl(&X1);
    uint32_t a = est < 1 ? 1 : (uint32_t)est;
    fx t;
    for (;;) {                             /* a = floor(X0 / X1) exactly */
        fx_mul_small(&t, &X1, a);
        if (fx_cmp(&t, &X0) > 0) { --a; continue; }
        fx u;
        fx_mul_small(&u, &X1, a + 1);
        if (fx_cmp(&u, &X0) <= 0) { ++a; continue; }
        break;
    }
    int sign_neg = !(level & 1);           /* (-1)^(i-1) for i = level: negative when i is even */
    fx m = X0;
    for (uint32_t j = 1; j <= a; ++j) {
        fx_sub(&m, &m, &X1);               /* x_(i-1) - j x_i */
        big q;
        big_addmul(&q, &Q0, &Q1, j);
        fx d;
        if (sign_neg) fx_neg(&d, &m); else d = m;
        add_cand(&q, &d);
    }
    fx nx;
    fx_mul_small(&t, &X1, a);
    fx_sub(&nx, &X0, &t);
    big nq;
    big_addmul(&nq, &Q0, &Q1, a);
    X0 = X1; X1 = nx;
    Q0 = Q1; Q1 = nq;
    ++level;
    return 1;
}

static big *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static big K;
static fx Fk;                              /* f(K) */

static void push_term(void)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? 2 * term_cap : 256;
        big *t = (big *)realloc(terms, term_cap * sizeof *t);
        if (!t) abort();
        terms = t;
    }
    terms[term_len++] = K;
}

static void init(void)
{
    fx pi, t;
    atan_inv(&pi, 5);
    fx_mul_small(&pi, &pi, 16);
    atan_inv(&t, 239);
    fx_mul_small(&t, &t, 4);
    fx_sub(&pi, &pi, &t);
    X0 = pi;                               /* x_-1 */
    memset(&X1, 0, sizeof X1);
    X1.d[FL] = 1;                          /* x_0 = 1 */
    memset(&Q0, 0, sizeof Q0);
    memset(&Q1, 0, sizeof Q1);
    Q1.d[0] = 1;
    add_cand(&Q1, &X1);                    /* q = 1, delta = +1 */
    memset(&K, 0, sizeof K);
    fx_div_small(&Fk, &pi, 2);
    fx_neg(&Fk, &Fk);                      /* f(0) = -pi/2 */
    push_term();
}

static int next_record(void)
{
    fx af, two_f;
    fx_abs(&af, &Fk);
    fx_mul_small(&two_f, &af, 2);
    int fneg = fx_neg_p(&Fk);
    for (size_t i = 0;; ++i) {
        while (i >= ncand) if (!more_cands()) return 0;
        const cand *c = &cands[i];
        if (fx_neg_p(&c->delta) == fneg) continue;
        fx ad;
        fx_abs(&ad, &c->delta);
        if (fx_cmp(&ad, &two_f) >= 0) continue;
        big_addmul(&K, &K, &c->q, 1);
        fx_add(&Fk, &Fk, &c->delta);
        push_term();
        return 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!terms) init();
    while (term_len < (size_t)n)
        if (!next_record()) return (seq_t)0;
    const big *k = &terms[n - 1];
    seq_t v = (seq_t)0;
    for (int i = QL - 1; i >= 0; --i) v = v * (seq_t)65536 * (seq_t)65536 + (seq_t)k->d[i];
    return v;
}
