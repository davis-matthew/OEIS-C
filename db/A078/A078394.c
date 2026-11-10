/*
 * A078394  Numbers k such that reverse(A004154(k)) + 1 is prime.
 * https://oeis.org/A078394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = n-th k >= 0 such that reverse(A004154(k)) + 1 is prime,
 *            where A004154(k) = k! with trailing zeros removed. Computed by a
 *            search over k with a private big-integer factorial and
 *            Miller-Rabin primality (bases 2..37 after trial division)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A078394_fl = 0;
static size_t A078394_flen = 0, A078394_fcap = 0;
static idx_t A078394_nextk = 0;
static idx_t *A078394_found = 0;
static size_t A078394_nfound = 0, A078394_foundcap = 0;

static int A078394_ge(const uint32_t *a, const uint32_t *b, int L) {
    for (int i = L - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] > b[i];
    }
    return 1;
}

static void A078394_sub(uint32_t *a, const uint32_t *b, int L) {
    int64_t br = 0;
    for (int i = 0; i < L; i++) {
        int64_t d = (int64_t)a[i] - (int64_t)b[i] - br;
        if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)d;
    }
}

static void A078394_mm(uint32_t *r, const uint32_t *a, const uint32_t *b,
                       const uint32_t *n, int L, uint32_t ninv, uint32_t *t) {
    for (int i = 0; i < L + 2; i++) t[i] = 0;
    for (int i = 0; i < L; i++) {
        uint64_t c = 0, s;
        for (int j = 0; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + c;
            t[j] = (uint32_t)s;
            c = s >> 32;
        }
        s = (uint64_t)t[L] + c;
        t[L] = (uint32_t)s;
        t[L + 1] = (uint32_t)(s >> 32);
        uint32_t m = t[0] * ninv;
        s = (uint64_t)t[0] + (uint64_t)m * n[0];
        c = s >> 32;
        for (int j = 1; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)m * n[j] + c;
            t[j - 1] = (uint32_t)s;
            c = s >> 32;
        }
        s = (uint64_t)t[L] + c;
        t[L - 1] = (uint32_t)s;
        c = s >> 32;
        t[L] = t[L + 1] + (uint32_t)c;
    }
    if (t[L] != 0 || A078394_ge(t, n, L)) A078394_sub(t, n, L);
    for (int i = 0; i < L; i++) r[i] = t[i];
}

static void A078394_dbl(uint32_t *x, const uint32_t *n, int L) {
    uint32_t c = 0;
    for (int i = 0; i < L; i++) {
        uint32_t nc = x[i] >> 31;
        x[i] = (x[i] << 1) | c;
        c = nc;
    }
    if (c || A078394_ge(x, n, L)) A078394_sub(x, n, L);
}

static bool A078394_prime(const uint32_t *n, int L) {
    for (uint32_t p = 2; p < 1000; p++) {
        bool pp = true;
        for (uint32_t q = 2; q * q <= p; q++) if (p % q == 0) { pp = false; break; }
        if (!pp) continue;
        uint64_t r = 0;
        for (int i = L - 1; i >= 0; i--) r = ((r << 32) | n[i]) % p;
        if (r == 0) return L == 1 && n[0] == p;
    }
    if (L == 1) {
        if (n[0] < 1000000u) return n[0] >= 2;
    }
    uint32_t n0 = n[0], x = n0;
    for (int i = 0; i < 5; i++) x *= 2u - n0 * x;
    uint32_t ninv = (uint32_t)(0u - x);
    uint32_t *buf = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(8 * L + 8));
    uint32_t *one = buf, *r2 = buf + L, *nm1m = buf + 2 * L, *xx = buf + 3 * L,
             *am = buf + 4 * L, *d = buf + 5 * L, *t = buf + 6 * L, *sm = buf + 6 * L + L + 2;
    for (int i = 0; i < L; i++) one[i] = 0;
    one[0] = 1;
    for (int i = 0; i < 32 * L; i++) A078394_dbl(one, n, L);
    for (int i = 0; i < L; i++) r2[i] = one[i];
    for (int i = 0; i < 32 * L; i++) A078394_dbl(r2, n, L);
    for (int i = 0; i < L; i++) nm1m[i] = n[i];
    A078394_sub(nm1m, one, L);
    /* d = (n-1) >> s */
    for (int i = 0; i < L; i++) d[i] = n[i];
    d[0] -= 1;
    int s = 0;
    while (!(d[0] & 1)) {
        for (int i = 0; i < L; i++) d[i] = (d[i] >> 1) | (i + 1 < L ? d[i + 1] << 31 : 0);
        s++;
    }
    int top = L * 32 - 1;
    while (top >= 0 && !((d[top / 32] >> (top % 32)) & 1)) top--;
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    bool prime = true;
    for (int bi = 0; bi < 12 && prime; bi++) {
        for (int i = 0; i < L; i++) sm[i] = 0;
        sm[0] = bases[bi];
        A078394_mm(am, sm, r2, n, L, ninv, t);
        for (int i = 0; i < L; i++) xx[i] = one[i];
        for (int b = top; b >= 0; b--) {
            A078394_mm(xx, xx, xx, n, L, ninv, t);
            if ((d[b / 32] >> (b % 32)) & 1) A078394_mm(xx, xx, am, n, L, ninv, t);
        }
        bool ok = (memcmp(xx, one, sizeof(uint32_t) * L) == 0) ||
                  (memcmp(xx, nm1m, sizeof(uint32_t) * L) == 0);
        for (int r = 1; r < s && !ok; r++) {
            A078394_mm(xx, xx, xx, n, L, ninv, t);
            if (memcmp(xx, nm1m, sizeof(uint32_t) * L) == 0) ok = true;
        }
        if (!ok) prime = false;
    }
    free(buf);
    return prime;
}

static bool A078394_test(void) {
    size_t nd = A078394_flen * 9 + 2;
    char *dig = (char *)malloc(nd);
    size_t len = 0;
    for (size_t i = A078394_flen; i-- > 0;) {
        uint32_t v = A078394_fl[i];
        char tmp[9];
        int c = 0;
        for (int j = 0; j < 9; j++) { tmp[c++] = (char)('0' + v % 10); v /= 10; }
        if (i == A078394_flen - 1) while (c > 1 && tmp[c - 1] == '0') c--;
        while (c > 0) dig[len++] = tmp[--c];
    }
    while (len > 1 && dig[len - 1] == '0') len--;
    size_t cap = len / 9 + 3;
    uint32_t *x = (uint32_t *)malloc(sizeof(uint32_t) * cap);
    size_t xl = 1;
    x[0] = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t carry = (uint64_t)(dig[i] - '0');
        for (size_t j = 0; j < xl; j++) {
            uint64_t v = (uint64_t)x[j] * 10 + carry;
            x[j] = (uint32_t)v;
            carry = v >> 32;
        }
        if (carry) x[xl++] = (uint32_t)carry;
    }
    {
        size_t j = 0;
        while (j < xl) {
            x[j]++;
            if (x[j] != 0) break;
            j++;
        }
        if (j == xl) x[xl++] = 1;
    }
    bool res = A078394_prime(x, (int)xl);
    free(x);
    free(dig);
    return res;
}

static void A078394_advance(void) {
    if (!A078394_fl) {
        A078394_fcap = 16;
        A078394_fl = (uint32_t *)malloc(sizeof(uint32_t) * A078394_fcap);
        A078394_fl[0] = 1;
        A078394_flen = 1;
        A078394_nextk = 0;
    }
    idx_t k = A078394_nextk;
    if (A078394_test()) {
        if (A078394_nfound == A078394_foundcap) {
            A078394_foundcap = A078394_foundcap ? A078394_foundcap * 2 : 16;
            A078394_found = (idx_t *)realloc(A078394_found, sizeof(idx_t) * A078394_foundcap);
        }
        A078394_found[A078394_nfound++] = k;
    }
    A078394_nextk = k + 1;
    uint64_t m = (uint64_t)(k + 1);
    uint64_t carry = 0;
    for (size_t j = 0; j < A078394_flen; j++) {
        uint64_t v = (uint64_t)A078394_fl[j] * m + carry;
        A078394_fl[j] = (uint32_t)(v % 1000000000u);
        carry = v / 1000000000u;
    }
    while (carry) {
        if (A078394_flen == A078394_fcap) {
            A078394_fcap *= 2;
            A078394_fl = (uint32_t *)realloc(A078394_fl, sizeof(uint32_t) * A078394_fcap);
        }
        A078394_fl[A078394_flen++] = (uint32_t)(carry % 1000000000u);
        carry /= 1000000000u;
    }
}

seq_t a(idx_t n) {
    while (A078394_nfound < (size_t)n) A078394_advance();
    return (seq_t)A078394_found[n - 1];
}
