/*
 * A000251  Number of trees of diameter 6.
 * https://oeis.org/A000251
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R3(n) - R2(n) - [x^n] (R2(x)-R1(x))*R2(x), with R1 =
 *            x/(1-x), R2(n) = p(n-1) (p = partition numbers, A000041), R3(n)
 *            = c(n-1), where c = Euler transform of R2: c(0) = 1, c(m) =
 *            (1/m) * Sum_{k=1..m} c(m-k) * Sum_{d|k} d*p(d-1). (Rooted at the
 *            center: height-3 rooted trees minus those with f...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 43, u64 n <= 95, u128 n <= 214,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 u128_t;
typedef struct { uint64_t *w; int len; } big;

static big *P_ = NULL, *D_ = NULL, *C_ = NULL;
static long long have = 0, cap = 0;
static uint64_t *buf = NULL;
static long long bufcap = 0;

static void ensure_buf(long long sz)
{
    if (sz > bufcap) {
        buf = (uint64_t *)realloc(buf, (size_t)sz * sizeof(uint64_t));
        bufcap = sz;
    }
}

static big mkbig(const uint64_t *a, int len)
{
    big r;
    while (len > 0 && a[len - 1] == 0) len--;
    r.len = len;
    r.w = (uint64_t *)malloc((size_t)(len ? len : 1) * sizeof(uint64_t));
    if (len) memcpy(r.w, a, (size_t)len * sizeof(uint64_t));
    return r;
}

static void add_mul_small(uint64_t *acc, const big *a, uint64_t m)
{
    u128_t carry = 0;
    int i;
    for (i = 0; i < a->len; i++) {
        u128_t t = (u128_t)a->w[i] * m + acc[i] + carry;
        acc[i] = (uint64_t)t;
        carry = t >> 64;
    }
    while (carry) {
        u128_t t = (u128_t)acc[i] + carry;
        acc[i] = (uint64_t)t;
        carry = t >> 64;
        i++;
    }
}

static void mac(uint64_t *acc, const big *a, const big *b)
{
    int i, j, k;
    for (i = 0; i < a->len; i++) {
        u128_t carry = 0;
        uint64_t ai = a->w[i];
        for (j = 0; j < b->len; j++) {
            u128_t t = (u128_t)ai * b->w[j] + acc[i + j] + carry;
            acc[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
        k = i + b->len;
        while (carry) {
            u128_t t = (u128_t)acc[k] + carry;
            acc[k] = (uint64_t)t;
            carry = t >> 64;
            k++;
        }
    }
}

static void extend(long long N)
{
    long long m;
    if (N < have) return;
    if (N + 1 > cap) {
        long long nc = 2 * cap;
        if (nc < N + 1) nc = N + 1;
        P_ = (big *)realloc(P_, (size_t)nc * sizeof(big));
        D_ = (big *)realloc(D_, (size_t)nc * sizeof(big));
        C_ = (big *)realloc(C_, (size_t)nc * sizeof(big));
        cap = nc;
    }
    for (m = have; m <= N; m++) {
        uint64_t one = 1;
        if (m == 0) {
            P_[0] = mkbig(&one, 1);
            D_[0] = mkbig(&one, 0);
            C_[0] = mkbig(&one, 1);
            continue;
        }
        /* partitions via pentagonal numbers */
        {
            int L = P_[m - 1].len + 2, i;
            long long j;
            uint64_t *pos, *neg;
            uint64_t borrow = 0;
            ensure_buf(2LL * L);
            pos = buf;
            neg = buf + L;
            memset(buf, 0, (size_t)(2 * L) * sizeof(uint64_t));
            for (j = 1;; j++) {
                long long g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
                uint64_t *t = (j & 1) ? pos : neg;
                if (g1 > m) break;
                add_mul_small(t, &P_[m - g1], 1);
                if (g2 <= m) add_mul_small(t, &P_[m - g2], 1);
            }
            for (i = 0; i < L; i++) {
                uint64_t x = pos[i], y = neg[i];
                uint64_t r = x - y - borrow;
                borrow = (x < y || (x == y && borrow)) ? 1 : 0;
                pos[i] = r;
            }
            P_[m] = mkbig(pos, L);
        }
        /* d(m) = Sum_{e|m} e * p(e-1) */
        {
            int L = P_[m - 1].len + 3;
            long long e;
            ensure_buf(L);
            memset(buf, 0, (size_t)L * sizeof(uint64_t));
            for (e = 1; e * e <= m; e++) {
                if (m % e == 0) {
                    long long f = m / e;
                    add_mul_small(buf, &P_[e - 1], (uint64_t)e);
                    if (f != e) add_mul_small(buf, &P_[f - 1], (uint64_t)f);
                }
            }
            D_[m] = mkbig(buf, L);
        }
        /* c(m) = (1/m) Sum_{k=1..m} d(k) c(m-k) */
        {
            int L = 0, i;
            long long k;
            u128_t rem = 0;
            for (k = 1; k <= m; k++) {
                int s = D_[k].len + C_[m - k].len;
                if (s > L) L = s;
            }
            L += 2;
            ensure_buf(L);
            memset(buf, 0, (size_t)L * sizeof(uint64_t));
            for (k = 1; k <= m; k++) mac(buf, &D_[k], &C_[m - k]);
            for (i = L - 1; i >= 0; i--) {
                u128_t cur = (rem << 64) | buf[i];
                buf[i] = (uint64_t)(cur / (uint64_t)m);
                rem = cur % (uint64_t)m;
            }
            C_[m] = mkbig(buf, L);
        }
    }
    have = N + 1;
}

static seq_t tos(const big *a)
{
    seq_t v = 0;
    int i, s;
    for (i = a->len - 1; i >= 0; i--) {
        for (s = 0; s < 8; s++) v <<= 8;
        v += a->w[i];
    }
    return v;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n, i;
    seq_t res, conv = 0;
    if (nn < 7) return 0;
    extend(nn - 1);
    for (i = 3; i <= nn - 1; i++) {
        seq_t t = tos(&P_[i - 1]);
        t -= 1;
        t *= tos(&P_[nn - i - 1]);
        conv += t;
    }
    res = tos(&C_[nn - 1]);
    res -= tos(&P_[nn - 1]);
    res -= conv;
    return res;
}
