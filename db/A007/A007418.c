/*
 * A007418  Numerators of expansion of exp x / sin x.
 * https://oeis.org/A007418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of c(n) = [x^n] exp(x)/sin(x), where c(n) =
 *            (1/(n+1)!) * Sum_{k=0..floor((n+1)/2)} binomial(n+1,2k) *
 *            |(4^k-2)*B(2k)|, B = Bernoulli numbers; |(4^k-2)*B(2k)| =
 *            k*(2^(2k-1)-1)*t(2k-1)/(2^(2k-2)*(4^k-1)) with t = tangent
 *            numbers. Computed exactly with internal multi-limb integers s...
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; } big;

static size_t L = 0;

static big bnew(void) {
    big x;
    x.d = (uint32_t *)calloc(L, sizeof(uint32_t));
    x.n = 0;
    return x;
}
static void bnorm(big *x) { while (x->n && x->d[x->n - 1] == 0) x->n--; }
static void bset(big *x, uint32_t v) { x->d[0] = v; x->n = v ? 1 : 0; }
static void bcopy(big *dst, const big *src) {
    memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}
static void bmul(big *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x->d[x->n++] = (uint32_t)c;
    bnorm(x);
}
static void baddmul(big *x, const big *y, uint32_t m) {
    size_t i;
    uint64_t c = 0;
    if (m == 0 || y->n == 0) return;
    while (x->n < y->n) x->d[x->n++] = 0;
    for (i = 0; i < y->n; i++) {
        uint64_t t = (uint64_t)y->d[i] * m + x->d[i] + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        if (i == x->n) { x->d[x->n++] = (uint32_t)c; break; }
        {
            uint64_t t = (uint64_t)x->d[i] + c;
            x->d[i] = (uint32_t)t;
            c = t >> 32;
        }
        i++;
    }
}
static void bsub(big *x, const big *y) {
    size_t i;
    uint32_t br = 0;
    for (i = 0; i < y->n; i++) {
        uint32_t a = x->d[i], b = y->d[i];
        uint32_t r = a - b - br;
        br = (a < b) || (a == b && br) ? 1u : 0u;
        x->d[i] = r;
    }
    for (; br && i < x->n; i++) {
        uint32_t a = x->d[i];
        x->d[i] = a - 1;
        br = (a == 0) ? 1u : 0u;
    }
    bnorm(x);
}
static uint32_t bmod(const big *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i > 0) {
        i--;
        r = ((r << 32) | x->d[i]) % p;
    }
    return (uint32_t)r;
}
static void bdiv(big *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / p);
        r = cur % p;
    }
    bnorm(x);
}
static void bshl(big *dst, const big *src, size_t s) {
    size_t w = s / 32, i;
    unsigned b = (unsigned)(s % 32);
    if (src->n == 0) { dst->n = 0; return; }
    for (i = 0; i < w; i++) dst->d[i] = 0;
    if (b == 0) {
        for (i = 0; i < src->n; i++) dst->d[i + w] = src->d[i];
        dst->n = src->n + w;
    } else {
        uint32_t carry = 0;
        for (i = 0; i < src->n; i++) {
            dst->d[i + w] = (src->d[i] << b) | carry;
            carry = src->d[i] >> (32 - b);
        }
        dst->d[src->n + w] = carry;
        dst->n = src->n + w + 1;
    }
    bnorm(dst);
}
static void bshr(big *x, size_t s) {
    size_t w = s / 32, i;
    unsigned b = (unsigned)(s % 32);
    if (w >= x->n) { x->n = 0; return; }
    for (i = 0; i + w < x->n; i++) {
        uint32_t lo = x->d[i + w] >> b;
        if (b && i + w + 1 < x->n) lo |= x->d[i + w + 1] << (32 - b);
        x->d[i] = lo;
    }
    x->n -= w;
    bnorm(x);
}
/* a <- a / b, exact division, b odd */
static void bdivexact(big *a, const big *b) {
    uint32_t b0 = b->d[0], inv = b0;
    size_t qlen, i, j;
    int it;
    if (a->n == 0) return;
    if (a->n < b->n) { a->n = 0; return; }
    for (it = 0; it < 5; it++) inv *= 2u - b0 * inv;
    qlen = a->n - b->n + 1;
    for (i = 0; i < qlen; i++) {
        uint32_t q = a->d[i] * inv;
        uint64_t c = 0;
        size_t k;
        for (j = 0; j < b->n; j++) {
            uint64_t p = (uint64_t)q * b->d[j] + c;
            uint32_t lo = (uint32_t)p, ai = a->d[i + j];
            c = p >> 32;
            a->d[i + j] = ai - lo;
            c += (ai < lo) ? 1u : 0u;
        }
        k = i + b->n;
        while (c && k < a->n) {
            uint32_t lo = (uint32_t)c, ai = a->d[k];
            uint64_t hi = c >> 32;
            a->d[k] = ai - lo;
            c = hi + ((ai < lo) ? 1u : 0u);
            k++;
        }
        a->d[i] = q;
    }
    a->n = qlen;
    bnorm(a);
}

static uint32_t **nums = NULL;
static size_t *nlens = NULL;
static idx_t have = 0; /* terms 0..have-1 known */

static size_t bitlen(size_t v) { size_t b = 0; while (v) { b++; v >>= 1; } return b; }

static void build(idx_t N) {
    size_t M = (size_t)N + 2, np = 0, i, j, k, s;
    size_t K = ((size_t)N + 1) / 2;
    size_t bits = 0;
    char *sv = (char *)calloc(M + 1, 1);
    uint32_t *pr = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
    big *T, *r, D, X, B, W;

    for (i = 2; i <= M; i++) {
        if (sv[i]) continue;
        pr[np++] = (uint32_t)i;
        for (j = i * i; j <= M; j += i) sv[j] = 1;
    }
    for (i = 0; i < np; i++) if (pr[i] != 2) bits += bitlen(pr[i]);
    for (i = 2; i <= M; i++) bits += bitlen(i);
    bits += 2 * (size_t)N + 160;
    L = bits / 32 + 4;

    T = (big *)malloc((K + 1) * sizeof(big));
    for (k = 1; k <= K; k++) T[k] = bnew();
    if (K >= 1) {
        bset(&T[1], 1);
        for (k = 2; k <= K; k++) { bcopy(&T[k], &T[k - 1]); bmul(&T[k], (uint32_t)(k - 1)); }
        for (k = 2; k <= K; k++)
            for (j = k; j <= K; j++) {
                bmul(&T[j], (uint32_t)(j - k + 2));
                if (j > k) baddmul(&T[j], &T[j - 1], (uint32_t)(j - k));
            }
    }

    D = bnew();
    bset(&D, 1);
    for (i = 0; i < np; i++) if (pr[i] != 2) bmul(&D, pr[i]);

    r = (big *)malloc((M) * sizeof(big));
    for (j = 0; j < M; j++) r[j] = bnew();
    bcopy(&r[0], &D);
    X = bnew();
    B = bnew();
    for (k = 1; k <= K; k++) {
        size_t full, rem;
        big *e = &r[2 * k];
        bcopy(&X, &T[k]);
        bmul(&X, (uint32_t)k);
        bshl(e, &X, 2 * k - 1);
        bsub(e, &X);
        bshr(e, 2 * k - 2);
        for (i = 0; i < np; i++) if (pr[i] != 2) bmul(e, pr[i]);
        full = (2 * k) / 32;
        rem = (2 * k) % 32;
        for (i = 0; i < full; i++) B.d[i] = 0xffffffffu;
        B.n = full;
        if (rem) { B.d[full] = (uint32_t)((1u << rem) - 1u); B.n++; }
        bdivexact(e, &B);
    }
    for (k = 1; k <= K; k++) free(T[k].d);
    free(T);
    free(X.d);
    free(B.d);
    free(D.d);

    nums = (uint32_t **)malloc(((size_t)N + 1) * sizeof(uint32_t *));
    nlens = (size_t *)malloc(((size_t)N + 1) * sizeof(size_t));
    W = bnew();
    for (s = 1; s <= (size_t)N + 1; s++) {
        size_t m = s - 1;
        for (j = 0; j + s <= (size_t)N + 1; j++) baddmul(&r[j], &r[j + 1], 1);
        bcopy(&W, &r[0]);
        for (i = 0; i < np; i++) {
            uint32_t p = pr[i];
            size_t e = (p != 2) ? 1 : 0, q = m + 1;
            while (q) { q /= p; e += q; }
            while (e && bmod(&W, p) == 0) { bdiv(&W, p); e--; }
        }
        nlens[m] = W.n;
        nums[m] = (uint32_t *)malloc((W.n ? W.n : 1) * sizeof(uint32_t));
        memcpy(nums[m], W.d, W.n * sizeof(uint32_t));
    }
    free(W.d);
    for (j = 0; j < M; j++) free(r[j].d);
    free(r);
    free(sv);
    free(pr);
    have = N + 1;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    size_t i;
    if (n >= have) {
        idx_t N = n, old = have;
        if (N < 2 * old) N = 2 * old;
        if (N < 64) N = 64;
        if (nums) {
            for (i = 0; i < (size_t)old; i++) free(nums[i]);
            free(nums);
            free(nlens);
            nums = NULL;
            nlens = NULL;
        }
        build(N);
    }
    i = nlens[n];
    while (i > 0) {
        int sh;
        uint32_t d;
        i--;
        d = nums[n][i];
        for (sh = 24; sh >= 0; sh -= 8) {
            res = res * 16;
            res = res * 16;
            res = res + (int)((d >> sh) & 255u);
        }
    }
    return res;
}
