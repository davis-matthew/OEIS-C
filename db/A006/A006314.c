/*
 * A006314  Numbers k such that k^8 + 1 is prime.
 * https://oeis.org/A006314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search over k (k = 1, 2 and even k), testing k^8+1 for primality
 *            (trial division by primes == 1 mod 16, then Miller-Rabin on
 *            multi-limb integers)
 * Range:     u8 n <= 10, u16 n <= 1719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   eq_limbs, geq_limbs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/eq_limbs.h"
#include "../utils/geq_limbs.h"

typedef unsigned __int128 u128_t;

static uint64_t *tdp = NULL;
static int tdp_count = 0;

static void init_trial_primes(void) {
    const int lim = 4000;
    char *comp = (char *)calloc(lim + 1, 1);
    int i, j;
    tdp = (uint64_t *)malloc(sizeof(uint64_t) * (lim + 1));
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        if (i % 16 == 1) tdp[tdp_count++] = (uint64_t)i;
    }
    free(comp);
}

static void sub_limbs(uint64_t *x, const uint64_t *y, int m) {
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < m; i++) {
        uint64_t yi = y[i] + borrow;
        uint64_t nb = (yi < borrow) || (x[i] < yi);
        x[i] -= yi;
        borrow = nb;
    }
}

static void dbl_mod(uint64_t *r, const uint64_t *n, int m) {
    uint64_t carry = r[m - 1] >> 63;
    int i;
    for (i = m - 1; i > 0; i--) r[i] = (r[i] << 1) | (r[i - 1] >> 63);
    r[0] <<= 1;
    if (carry || geq_limbs(r, n, m)) sub_limbs(r, n, m);
}

static void mont_mul(uint64_t *r, const uint64_t *x, const uint64_t *y,
                     const uint64_t *n, uint64_t ninv, int m) {
    uint64_t t[6] = {0, 0, 0, 0, 0, 0};
    int i, j;
    for (i = 0; i < m; i++) {
        u128_t c = 0;
        uint64_t q;
        for (j = 0; j < m; j++) {
            c += (u128_t)x[j] * y[i] + t[j];
            t[j] = (uint64_t)c;
            c >>= 64;
        }
        c += t[m];
        t[m] = (uint64_t)c;
        t[m + 1] = (uint64_t)(c >> 64);
        q = t[0] * ninv;
        c = (u128_t)q * n[0] + t[0];
        c >>= 64;
        for (j = 1; j < m; j++) {
            c += (u128_t)q * n[j] + t[j];
            t[j - 1] = (uint64_t)c;
            c >>= 64;
        }
        c += t[m];
        t[m - 1] = (uint64_t)c;
        t[m] = t[m + 1] + (uint64_t)(c >> 64);
    }
    if (t[m] || geq_limbs(t, n, m)) sub_limbs(t, n, m);
    memcpy(r, t, sizeof(uint64_t) * m);
}

/* n odd, > 2^32, m limbs (m <= 4) */
static bool is_prime_big(const uint64_t *n, int m) {
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t inv = n[0], ninv;
    uint64_t r2[4], one[4], minus1[4], plain[4], x[4], bm[4], nm1[4];
    int i, s, top, b, it;

    for (i = 0; i < 6; i++) inv *= 2 - n[0] * inv;
    ninv = (uint64_t)0 - inv;

    memset(r2, 0, sizeof r2);
    r2[0] = 1;
    for (i = 0; i < 128 * m; i++) dbl_mod(r2, n, m);

    memset(plain, 0, sizeof plain);
    plain[0] = 1;
    mont_mul(one, r2, plain, n, ninv, m);
    memcpy(minus1, n, sizeof(uint64_t) * m);
    sub_limbs(minus1, one, m);

    memcpy(nm1, n, sizeof(uint64_t) * m);
    nm1[0] -= 1; /* n odd, no borrow */
    s = 0;
    while (((nm1[s / 64] >> (s % 64)) & 1) == 0) s++;
    top = 64 * m - 1;
    while (((nm1[top / 64] >> (top % 64)) & 1) == 0) top--;

    for (b = 0; b < (int)(sizeof(bases) / sizeof(bases[0])); b++) {
        memset(plain, 0, sizeof plain);
        plain[0] = bases[b];
        mont_mul(bm, r2, plain, n, ninv, m);
        memcpy(x, one, sizeof(uint64_t) * m);
        for (i = top; i >= s; i--) {
            mont_mul(x, x, x, n, ninv, m);
            if ((nm1[i / 64] >> (i % 64)) & 1) mont_mul(x, x, bm, n, ninv, m);
        }
        if (eq_limbs(x, one, m) || eq_limbs(x, minus1, m)) continue;
        for (it = 1; it < s; it++) {
            mont_mul(x, x, x, n, ninv, m);
            if (eq_limbs(x, minus1, m)) break;
            if (eq_limbs(x, one, m)) return false;
        }
        if (it >= s) return false;
    }
    return true;
}

static bool k8p1_prime(uint64_t k) {
    uint64_t v[4], k4[2], k2;
    u128_t k4w;
    int i, j, m;
    if (k == 1 || k == 2) return true;
    if (k & 1) return false;
    for (i = 0; i < tdp_count; i++) {
        uint64_t p = tdp[i];
        uint64_t t = k % p;
        t = t * t % p;
        t = t * t % p;
        t = t * t % p;
        if (t == p - 1) return false;
    }
    k2 = k * k;
    k4w = (u128_t)k2 * k2;
    k4[0] = (uint64_t)k4w;
    k4[1] = (uint64_t)(k4w >> 64);
    memset(v, 0, sizeof v);
    for (i = 0; i < 2; i++) {
        u128_t c = 0;
        for (j = 0; j < 2; j++) {
            c += (u128_t)k4[i] * k4[j] + v[i + j];
            v[i + j] = (uint64_t)c;
            c >>= 64;
        }
        v[i + 2] += (uint64_t)c;
    }
    for (i = 0; i < 4; i++) {
        v[i] += 1;
        if (v[i] != 0) break;
    }
    m = 4;
    while (m > 1 && v[m - 1] == 0) m--;
    return is_prime_big(v, m);
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static idx_t count = 0, cap = 0;
    static uint64_t lastk = 0;
    seq_t r = 0;

    if (tdp == NULL) init_trial_primes();
    while (count < n) {
        lastk++;
        if (k8p1_prime(lastk)) {
            if (count >= cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, sizeof(uint64_t) * nc);
                cap = nc;
            }
            terms[count++] = lastk;
        }
    }
    r += (idx_t)terms[n - 1];
    return r;
}
