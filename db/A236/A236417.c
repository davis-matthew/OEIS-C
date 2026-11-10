/*
 * A236417  a(n) = |{0 < k < n: p = phi(k)/2 + phi(n-k)/12 + 1 and A047967(p)
 *          are both prime}|.
 * https://oeis.org/A236417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = phi(k)/2 + phi(n-k)/12 + 1 is prime and
 *            A047967(p) is prime}|. Computed by a search over k: a phi sieve,
 *            exact multi-precision partition numbers p(m) from Euler's
 *            pentagonal recurrence, A047967(p) = p(p) - q(p) with q(p) =
 *            Sum_{j in Z} (-1)^j p(p - j(3j-1)), and a Miller-...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9022 (2026-11-07)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

static uint32_t *A236417_P = NULL;       /* partition numbers, L limbs each */
static unsigned char *A236417_flag = NULL;
static idx_t A236417_cap = 0;
static size_t A236417_L = 0;

static void A236417_add(uint32_t *r, const uint32_t *a, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; ++i) {
        uint64_t s = (uint64_t)r[i] + a[i] + c;
        r[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void A236417_sub(uint32_t *r, const uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t br = 0;
    for (size_t i = 0; i < L; ++i) {
        uint64_t s = (uint64_t)a[i] - b[i] - br;
        r[i] = (uint32_t)s;
        br = (s >> 63) & 1;
    }
}

static int A236417_cmp(const uint32_t *a, const uint32_t *b, size_t k)
{
    for (size_t i = k; i-- > 0; ) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A236417_build(idx_t need)
{
    idx_t cap = A236417_cap ? A236417_cap : 64;
    while (cap <= need) cap <<= 1;
    uint64_t s = isqrt_u64((uint64_t)cap) + 1;
    size_t L = (size_t)((4 * s + 64) / 32 + 1);

    uint32_t *P = (uint32_t *)calloc((size_t)cap * L, sizeof(uint32_t));
    uint32_t *pos = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)malloc(L * sizeof(uint32_t));
    P[0] = 1;
    for (idx_t m = 1; m < cap; ++m) {
        memset(pos, 0, L * sizeof(uint32_t));
        memset(neg, 0, L * sizeof(uint32_t));
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            uint32_t *tg = (k & 1) ? pos : neg;
            A236417_add(tg, P + (size_t)(m - g1) * L, L);
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) A236417_add(tg, P + (size_t)(m - g2) * L, L);
        }
        A236417_sub(P + (size_t)m * L, pos, neg, L);
    }
    free(pos);
    free(neg);
    if (A236417_P) free(A236417_P);
    A236417_P = P;

    unsigned char *fl = (unsigned char *)calloc((size_t)cap, 1);
    if (A236417_flag) {
        memcpy(fl, A236417_flag, (size_t)A236417_cap);
        free(A236417_flag);
    }
    A236417_flag = fl;
    A236417_L = L;
    A236417_cap = cap;
}

static void A236417_mm(uint32_t *r, const uint32_t *a, const uint32_t *b,
                       const uint32_t *m, size_t k, uint32_t mp, uint32_t *t)
{
    memset(t, 0, (k + 2) * sizeof(uint32_t));
    for (size_t i = 0; i < k; ++i) {
        uint64_t C = 0, S;
        for (size_t j = 0; j < k; ++j) {
            S = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + C;
            t[j] = (uint32_t)S;
            C = S >> 32;
        }
        S = (uint64_t)t[k] + C;
        t[k] = (uint32_t)S;
        t[k + 1] = (uint32_t)(S >> 32);
        uint32_t q = t[0] * mp;
        S = (uint64_t)t[0] + (uint64_t)q * m[0];
        C = S >> 32;
        for (size_t j = 1; j < k; ++j) {
            S = (uint64_t)t[j] + (uint64_t)q * m[j] + C;
            t[j - 1] = (uint32_t)S;
            C = S >> 32;
        }
        S = (uint64_t)t[k] + C;
        t[k - 1] = (uint32_t)S;
        t[k] = t[k + 1] + (uint32_t)(S >> 32);
    }
    if (t[k] || A236417_cmp(t, m, k) >= 0) A236417_sub(t, t, m, k);
    memcpy(r, t, k * sizeof(uint32_t));
}

/* Miller-Rabin for odd m with k >= 3 limbs */
static bool A236417_mr(const uint32_t *m, size_t k)
{
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    size_t W = k + 2;
    uint32_t *buf = (uint32_t *)calloc(8 * W, sizeof(uint32_t));
    uint32_t *R2 = buf, *one = buf + W, *mone = buf + 2 * W, *x = buf + 3 * W;
    uint32_t *bm = buf + 4 * W, *tmp = buf + 5 * W, *t = buf + 6 * W;
    bool result = true;

    uint32_t inv = m[0];
    for (int i = 0; i < 5; ++i) inv *= 2 - m[0] * inv;
    uint32_t mp = (uint32_t)(0u - inv);

    /* R2 = R^2 mod m by doubling 1 */
    R2[0] = 1;
    for (size_t i = 0; i < 64 * k; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < k; ++j) {
            uint32_t nc = R2[j] >> 31;
            R2[j] = (R2[j] << 1) | carry;
            carry = nc;
        }
        if (carry || A236417_cmp(R2, m, k) >= 0) A236417_sub(R2, R2, m, k);
    }
    memset(tmp, 0, k * sizeof(uint32_t));
    tmp[0] = 1;
    A236417_mm(one, tmp, R2, m, k, mp, t);
    A236417_sub(mone, m, one, k);

    /* m - 1 = d * 2^s */
    size_t sbits = 1;
    while (!((m[sbits / 32] >> (sbits % 32)) & 1)) ++sbits;
    size_t top = 32 * k - 1;
    while (!((m[top / 32] >> (top % 32)) & 1)) --top;

    for (int bi = 0; bi < 12 && result; ++bi) {
        memset(tmp, 0, k * sizeof(uint32_t));
        tmp[0] = bases[bi];
        A236417_mm(bm, tmp, R2, m, k, mp, t);
        memcpy(x, one, k * sizeof(uint32_t));
        for (size_t i = top + 1; i-- > sbits; ) {
            A236417_mm(x, x, x, m, k, mp, t);
            if ((m[i / 32] >> (i % 32)) & 1) A236417_mm(x, x, bm, m, k, mp, t);
        }
        if (A236417_cmp(x, one, k) == 0 || A236417_cmp(x, mone, k) == 0) continue;
        bool ok = false;
        for (size_t r = 1; r < sbits; ++r) {
            A236417_mm(x, x, x, m, k, mp, t);
            if (A236417_cmp(x, mone, k) == 0) { ok = true; break; }
            if (A236417_cmp(x, one, k) == 0) break;
        }
        if (!ok) result = false;
    }
    free(buf);
    return result;
}

static bool A236417_small_prime(uint64_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

/* is A047967(p) prime? */
static bool A236417_test(idx_t p)
{
    size_t L = A236417_L;
    uint32_t *pos = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (idx_t k = 1;; ++k) {
        idx_t g1 = k * (3 * k - 1);
        if (g1 > p) break;
        uint32_t *tg = (k & 1) ? pos : neg;
        A236417_add(tg, A236417_P + (size_t)(p - g1) * L, L);
        idx_t g2 = k * (3 * k + 1);
        if (g2 <= p) A236417_add(tg, A236417_P + (size_t)(p - g2) * L, L);
    }
    A236417_sub(pos, pos, neg, L);
    free(neg);
    size_t k = L;
    while (k > 0 && pos[k - 1] == 0) --k;
    bool res;
    if (k <= 2) {
        uint64_t v = (k >= 1 ? (uint64_t)pos[0] : 0) | (k == 2 ? ((uint64_t)pos[1] << 32) : 0);
        res = is_prime_u64(v);
    } else if (!(pos[0] & 1)) {
        res = false;
    } else {
        res = true;
        for (uint32_t q = 3; q < 1000 && res; q += 2) {
            if (!A236417_small_prime(q)) continue;
            uint64_t r = 0;
            for (size_t i = k; i-- > 0; ) r = ((r << 32) | pos[i]) % q;
            if (r == 0) res = false;
        }
        if (res) res = A236417_mr(pos, k);
    }
    free(pos);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t need = n * 7 / 12 + 4;
    if (need >= A236417_cap) A236417_build(need);

    uint32_t *phi = (uint32_t *)malloc(((size_t)n + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= n; ++i) phi[i] = (uint32_t)i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == (uint32_t)i) {
            for (idx_t j = i; j <= n; j += i) phi[j] -= phi[j] / (uint32_t)i;
        }
    }

    idx_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint64_t num = 6 * (uint64_t)phi[k] + (uint64_t)phi[n - k] + 12;
        if (num % 12) continue;
        uint64_t p = num / 12;
        if (p >= A236417_cap) continue;
        unsigned char f = A236417_flag[p];
        if (f == 0) {
            bool ok = false;
            if (A236417_small_prime(p)) ok = A236417_test((idx_t)p);
            f = ok ? 2 : 1;
            A236417_flag[p] = f;
        }
        if (f == 2) count++;
    }
    free(phi);
    return (seq_t)count;
}
