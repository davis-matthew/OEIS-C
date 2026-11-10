/*
 * A083400  Numbers n such that A053664(n) and A073605(n) are primes.
 * https://oeis.org/A083400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   search: a(n) = n-th k such that A053664(k) and A073605(k) are
 *            both prime; both are computed by incremental CRT (x == k resp. x
 *            == -k mod prime(i) for i <= k) in internal multiprecision,
 *            tested by Miller-Rabin
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod_u64.h"

typedef struct { uint32_t *d; size_t len, cap; } A083400_bn;

static void A083400_ensure(A083400_bn *b, size_t n)
{
    if (b->cap < n) {
        size_t nc = b->cap ? b->cap : 8;
        while (nc < n) nc <<= 1;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        if (!b->d) abort();
        memset(b->d + b->cap, 0, (nc - b->cap) * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void A083400_norm(A083400_bn *b)
{
    while (b->len > 0 && b->d[b->len - 1] == 0) b->len--;
}

static uint32_t A083400_modsmall(const A083400_bn *b, uint32_t p)
{
    uint64_t r = 0;
    size_t i = b->len;
    while (i--) r = ((r << 32) | b->d[i]) % p;
    return (uint32_t)r;
}

/* x += P * t */
static void A083400_addmul(A083400_bn *x, const A083400_bn *P, uint32_t t)
{
    size_t n = (P->len > x->len ? P->len : x->len) + 2;
    A083400_ensure(x, n);
    for (size_t i = x->len; i < n; ++i) x->d[i] = 0;
    uint64_t c = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t pi = i < P->len ? P->d[i] : 0;
        uint64_t s = (uint64_t)x->d[i] + pi * t + c;
        x->d[i] = (uint32_t)s;
        c = s >> 32;
    }
    x->len = n;
    A083400_norm(x);
}

static void A083400_mulsmall(A083400_bn *P, uint32_t p)
{
    A083400_ensure(P, P->len + 1);
    uint64_t c = 0;
    for (size_t i = 0; i < P->len; ++i) {
        uint64_t s = (uint64_t)P->d[i] * p + c;
        P->d[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c) P->d[P->len++] = (uint32_t)c;
}

static bool A083400_ge(const uint32_t *a, const uint32_t *b, size_t s)
{
    size_t i = s;
    while (i--) {
        if (a[i] != b[i]) return a[i] > b[i];
    }
    return true;
}

static void A083400_sub(uint32_t *a, const uint32_t *b, size_t s)
{
    uint64_t br = 0;
    for (size_t i = 0; i < s; ++i) {
        uint64_t v = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)v;
        br = (v >> 63) & 1;
    }
}

/* out = a*b*R^-1 mod N (Montgomery, CIOS); t has s+2 limbs */
static void A083400_mont(uint32_t *out, const uint32_t *a, const uint32_t *b,
                         const uint32_t *N, uint32_t n0inv, size_t s, uint32_t *t)
{
    for (size_t j = 0; j < s + 2; ++j) t[j] = 0;
    for (size_t i = 0; i < s; ++i) {
        uint64_t C = 0, cs;
        for (size_t j = 0; j < s; ++j) {
            cs = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + C;
            t[j] = (uint32_t)cs; C = cs >> 32;
        }
        cs = (uint64_t)t[s] + C;
        t[s] = (uint32_t)cs; t[s + 1] = (uint32_t)(cs >> 32);
        uint32_t m = t[0] * n0inv;
        cs = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = cs >> 32;
        for (size_t j = 1; j < s; ++j) {
            cs = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)cs; C = cs >> 32;
        }
        cs = (uint64_t)t[s] + C;
        t[s - 1] = (uint32_t)cs;
        t[s] = t[s + 1] + (uint32_t)(cs >> 32);
    }
    if (t[s] != 0 || A083400_ge(t, N, s)) A083400_sub(t, N, s);
    for (size_t j = 0; j < s; ++j) out[j] = t[j];
}

static bool A083400_eq(const uint32_t *a, const uint32_t *b, size_t s)
{
    for (size_t i = 0; i < s; ++i) if (a[i] != b[i]) return false;
    return true;
}

static bool A083400_isprime(const A083400_bn *Nb)
{
    static const uint32_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
        67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,
        173,179,181,191,193,197,199,211,223,227,229,233,239,241,251};
    size_t ns = sizeof(small) / sizeof(small[0]);
    size_t s = Nb->len;
    if (s == 0) return false;
    if (s == 1 && Nb->d[0] < 2) return false;
    for (size_t i = 0; i < ns; ++i) {
        if (A083400_modsmall(Nb, small[i]) == 0)
            return s == 1 && Nb->d[0] == small[i];
    }
    if (s == 1 && Nb->d[0] < 251u * 251u) return true;

    const uint32_t *N = Nb->d;
    uint32_t *buf = (uint32_t *)calloc(9 * s + 2, sizeof(uint32_t));
    if (!buf) abort();
    uint32_t *R2 = buf, *one = buf + s, *neg = buf + 2 * s, *x = buf + 3 * s;
    uint32_t *bm = buf + 4 * s, *tmp = buf + 5 * s, *nm1 = buf + 6 * s;
    uint32_t *t = buf + 7 * s; /* s+2 limbs */

    uint32_t N0 = N[0], inv = N0;
    for (int i = 0; i < 5; ++i) inv *= 2u - N0 * inv;
    uint32_t n0inv = 0u - inv;

    /* R2 = 2^(64 s) mod N */
    R2[0] = 1;
    for (size_t it = 0; it < 64 * s; ++it) {
        uint32_t carry = 0;
        for (size_t j = 0; j < s; ++j) {
            uint32_t nc = R2[j] >> 31;
            R2[j] = (R2[j] << 1) | carry;
            carry = nc;
        }
        if (carry || A083400_ge(R2, N, s)) A083400_sub(R2, N, s);
    }
    for (size_t j = 0; j < s; ++j) tmp[j] = 0;
    tmp[0] = 1;
    A083400_mont(one, tmp, R2, N, n0inv, s, t);
    for (size_t j = 0; j < s; ++j) neg[j] = N[j];
    A083400_sub(neg, one, s);

    for (size_t j = 0; j < s; ++j) nm1[j] = N[j];
    nm1[0] -= 1; /* N odd, no borrow */
    size_t r = 0;
    while (((nm1[r / 32] >> (r % 32)) & 1u) == 0) r++;
    size_t topbit = 32 * s;
    while (topbit > 0 && ((nm1[(topbit - 1) / 32] >> ((topbit - 1) % 32)) & 1u) == 0) topbit--;

    bool prime = true;
    for (size_t bi = 0; bi < 12 && prime; ++bi) {
        for (size_t j = 0; j < s; ++j) tmp[j] = 0;
        tmp[0] = small[bi];
        A083400_mont(bm, tmp, R2, N, n0inv, s, t);
        for (size_t j = 0; j < s; ++j) x[j] = one[j];
        for (size_t i = topbit; i > r; --i) {
            size_t bit = i - 1;
            A083400_mont(x, x, x, N, n0inv, s, t);
            if ((nm1[bit / 32] >> (bit % 32)) & 1u)
                A083400_mont(x, x, bm, N, n0inv, s, t);
        }
        if (A083400_eq(x, one, s) || A083400_eq(x, neg, s)) continue;
        bool ok = false;
        for (size_t i = 1; i < r; ++i) {
            A083400_mont(x, x, x, N, n0inv, s, t);
            if (A083400_eq(x, neg, s)) { ok = true; break; }
            if (A083400_eq(x, one, s)) break;
        }
        if (!ok) prime = false;
    }
    free(buf);
    return prime;
}

static uint32_t A083400_prime(size_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0, cap = 0;
    static uint32_t cand = 3;
    if (cap == 0) {
        cap = 64;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
        pr[sz++] = 2;
    }
    while (sz < k) {
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint64_t q = pr[i];
            if (q * q > cand) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) {
            if (sz == cap) {
                cap <<= 1;
                pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
                if (!pr) abort();
            }
            pr[sz++] = cand;
        }
        cand += 2;
    }
    return pr[k - 1];
}

seq_t a(idx_t n)
{
    static idx_t *found = NULL;
    static size_t nfound = 0, fcap = 0;
    static A083400_bn x1 = {NULL, 0, 0}, x2 = {NULL, 0, 0}, P = {NULL, 0, 0};
    static idx_t k = 0;

    if (P.len == 0) {
        A083400_ensure(&P, 8);
        P.d[0] = 1; P.len = 1;
        A083400_ensure(&x1, 8);
        A083400_ensure(&x2, 8);
    }

    while (nfound < (size_t)n) {
        k++;
        uint64_t p = (uint64_t)A083400_prime((size_t)k);
        uint64_t Pm = A083400_modsmall(&P, (uint32_t)p);
        uint64_t inv = powmod_u64(Pm, p - 2, p);
        uint64_t kp = (uint64_t)(k % p);
        uint64_t r1 = kp;
        uint64_t r2 = (p - kp) % p;
        uint64_t c1 = A083400_modsmall(&x1, (uint32_t)p);
        uint64_t c2 = A083400_modsmall(&x2, (uint32_t)p);
        uint64_t d1 = (r1 + p - c1) % p;
        uint64_t d2 = (r2 + p - c2) % p;
        uint32_t t1 = (uint32_t)((d1 * inv) % p);
        uint32_t t2 = (uint32_t)((d2 * inv) % p);

        A083400_addmul(&x1, &P, t1);
        A083400_addmul(&x2, &P, t2);
        A083400_mulsmall(&P, (uint32_t)p);

        if (A083400_isprime(&x1) && A083400_isprime(&x2)) {
            if (nfound == fcap) {
                fcap = fcap ? fcap * 2 : 16;
                found = (idx_t *)realloc(found, fcap * sizeof(idx_t));
                if (!found) abort();
            }
            found[nfound++] = k;
        }
    }
    return (seq_t)found[(size_t)n - 1];
}
