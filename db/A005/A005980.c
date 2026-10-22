/*
 * A005980  Let F(x) = 1 + x + 4x^2 + 9x^3 + ... = g.f. for A002835 (solid
 *          partitions restricted to two planes) and expand
 *          (1-x)*(1-x^2)*(1-x^3)*...*F(x) in powers of x.
 * https://oeis.org/A005980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Sum_{k} (-1)^k * F(n - k*(3k-1)/2) over generalized
 *            pentagonal numbers (Euler product), where F(m) = A002835(m)
 *            counts plane partitions whose entries are pairs (A>=B), computed
 *            by a memoized row-by-row transfer: each row is a 2-row plane
 *            partition, each row contained in the previous one
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *kpool = NULL;
static size_t kp_len = 0, kp_cap = 0;
static size_t *t_off = NULL;
static unsigned short *t_len = NULL;
static uint64_t *t_val = NULL;
static size_t t_cap = 0, t_cnt = 0;

static uint64_t *Fv = NULL;
static idx_t F_have = 0;

static uint64_t hkey(const unsigned char *k, size_t len)
{
    uint64_t h = 1469598103934665603ULL;
    size_t i;
    for (i = 0; i < len; i++) { h ^= k[i]; h *= 1099511628211ULL; }
    return h;
}

static size_t find_slot(const unsigned char *k, size_t len, uint64_t h)
{
    size_t m = t_cap - 1, i = (size_t)h & m;
    while (t_len[i] != 0) {
        if (t_len[i] == len && memcmp(kpool + t_off[i], k, len) == 0) return i;
        i = (i + 1) & m;
    }
    return i;
}

static void rehash(size_t ncap)
{
    size_t *oo = t_off;
    unsigned short *ol = t_len;
    uint64_t *ov = t_val;
    size_t oc = t_cap, i;
    t_cap = ncap;
    t_off = (size_t *)calloc(ncap, sizeof(size_t));
    t_len = (unsigned short *)calloc(ncap, sizeof(unsigned short));
    t_val = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    for (i = 0; i < oc; i++) {
        if (ol[i]) {
            const unsigned char *k = kpool + oo[i];
            size_t s = find_slot(k, ol[i], hkey(k, ol[i]));
            t_off[s] = oo[i]; t_len[s] = ol[i]; t_val[s] = ov[i];
        }
    }
    free(oo); free(ol); free(ov);
}

static void insert(const unsigned char *k, size_t len, uint64_t v)
{
    size_t s;
    if ((t_cnt + 1) * 2 > t_cap) rehash(t_cap ? t_cap * 2 : (size_t)1 << 16);
    s = find_slot(k, len, hkey(k, len));
    if (kp_len + len > kp_cap) {
        size_t nc = kp_cap ? kp_cap * 2 : (size_t)1 << 20;
        while (nc < kp_len + len) nc *= 2;
        kpool = (unsigned char *)realloc(kpool, nc);
        kp_cap = nc;
    }
    memcpy(kpool + kp_len, k, len);
    t_off[s] = kp_len; t_len[s] = (unsigned short)len; t_val[s] = v;
    kp_len += len;
    t_cnt++;
}

static uint64_t count(const unsigned char *sa, const unsigned char *sb, int L, int r);

struct ctx {
    unsigned char na[256], nb[256], ta[256], tb[256];
    int L, r;
    uint64_t total;
};

static void en(struct ctx *c, int j, int w)
{
    int amax, x, y, bmax;
    if (j > 0) c->total += count(c->ta, c->tb, j, c->r - w);
    if (j >= c->L) return;
    amax = c->na[j];
    if (j > 0 && c->ta[j - 1] < amax) amax = c->ta[j - 1];
    if (amax > c->r - w) amax = c->r - w;
    for (x = 1; x <= amax; x++) {
        bmax = x;
        if (c->nb[j] < bmax) bmax = c->nb[j];
        if (j > 0 && c->tb[j - 1] < bmax) bmax = c->tb[j - 1];
        if (c->r - w - x < bmax) bmax = c->r - w - x;
        for (y = 0; y <= bmax; y++) {
            c->ta[j] = (unsigned char)x;
            c->tb[j] = (unsigned char)y;
            en(c, j + 1, w + x + y);
        }
    }
}

/* number of ways to complete with rows each contained in (sa,sb), total weight exactly r */
static uint64_t count(const unsigned char *sa, const unsigned char *sb, int L, int r)
{
    struct ctx *c;
    unsigned char key[520];
    size_t klen, s;
    int j;
    uint64_t res;
    if (r == 0) return 1;
    c = (struct ctx *)malloc(sizeof(struct ctx));
    if (L > r) L = r;
    for (j = 0; j < L; j++) {
        int ca = r / (j + 1), cb = r / (2 * (j + 1));
        c->na[j] = (unsigned char)(sa[j] < ca ? sa[j] : ca);
        c->nb[j] = (unsigned char)(sb[j] < cb ? sb[j] : cb);
    }
    c->L = L; c->r = r; c->total = 0;
    key[0] = (unsigned char)r;
    key[1] = (unsigned char)L;
    memcpy(key + 2, c->na, (size_t)L);
    memcpy(key + 2 + L, c->nb, (size_t)L);
    klen = 2 + 2 * (size_t)L;
    if (t_cap) {
        s = find_slot(key, klen, hkey(key, klen));
        if (t_len[s]) { res = t_val[s]; free(c); return res; }
    }
    en(c, 0, 0);
    res = c->total;
    free(c);
    insert(key, klen, res);
    return res;
}

static void compute_F(idx_t n)
{
    idx_t m;
    unsigned char top[256];
    Fv = (uint64_t *)realloc(Fv, ((size_t)n + 1) * sizeof(uint64_t));
    for (m = F_have; m <= n; m++) {
        int mm = (int)m, j;
        for (j = 0; j < mm && j < 256; j++) top[j] = (unsigned char)(mm > 255 ? 255 : mm);
        Fv[m] = count(top, top, mm, mm);
    }
    F_have = n + 1;
}

seq_t a(idx_t n)
{
    uint64_t acc = 0;
    idx_t m;
    seq_t res = 0;
    if (n + 1 > F_have) compute_F(n);
    acc += Fv[n];
    for (m = 1;; m++) {
        idx_t k1 = m * (3 * m - 1) / 2;
        idx_t k2 = m * (3 * m + 1) / 2;
        if (k1 > n) break;
        if (m & 1) acc -= Fv[n - k1]; else acc += Fv[n - k1];
        if (k2 <= n) {
            if (m & 1) acc -= Fv[n - k2]; else acc += Fv[n - k2];
        }
    }
    res += (idx_t)acc;
    return res;
}
