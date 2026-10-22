/*
 * A006919  Write down all the prime divisors in previous term.
 * https://oeis.org/A006919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n+1) = A037276(a(n)), a(1) = 8 (concatenate prime factors of
 *            a(n) in increasing order with multiplicity); computed until it
 *            reaches a fixed point (a prime), after which a(n) is constant
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mulmod_u64, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"
#include "../utils/mulmod_u64.h"

static uint64_t powmod_u64(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod_u64(r, b, m);
        b = mulmod_u64(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, r, s;
    uint64_t d;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n % bases[i] == 0) return n == bases[i];
    }
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod_u64(bases[i], d, n);
        bool ok = false;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t rho(uint64_t n) {
    uint64_t c;
    if ((n & 1) == 0) return 2;
    for (c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod_u64(x, x, n) + c) % n;
            y = (mulmod_u64(y, y, n) + c) % n;
            y = (mulmod_u64(y, y, n) + c) % n;
            d = gcd_u64(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void factor_rec(uint64_t n, uint64_t *out, int *cnt) {
    uint64_t d;
    if (n == 1) return;
    if (mr_prime(n)) { out[(*cnt)++] = n; return; }
    d = rho(n);
    factor_rec(d, out, cnt);
    factor_rec(n / d, out, cnt);
}

static int cmp_u64(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static unsigned __int128 concat_factors(uint64_t n) {
    uint64_t f[128];
    int cnt = 0, i;
    uint64_t p;
    unsigned __int128 r = 0;
    for (p = 2; p < 1000 && p * p <= n; p++) {
        while (n % p == 0) { f[cnt++] = p; n /= p; }
    }
    if (n > 1) factor_rec(n, f, &cnt);
    qsort(f, (size_t)cnt, sizeof(uint64_t), cmp_u64);
    for (i = 0; i < cnt; i++) {
        unsigned __int128 pw = 1;
        uint64_t t = f[i];
        while (t > 0) { pw *= 10; t /= 10; }
        r = r * pw + f[i];
    }
    return r;
}

static uint64_t terms[256];
static int nterms = 0;

seq_t a(idx_t n) {
    if (nterms == 0) {
        terms[0] = 8;
        nterms = 1;
        while (nterms < 256) {
            unsigned __int128 nx = concat_factors(terms[nterms - 1]);
            if (nx > (unsigned __int128)UINT64_MAX) break;
            if ((uint64_t)nx == terms[nterms - 1]) break;
            terms[nterms++] = (uint64_t)nx;
        }
    }
    if (n >= (idx_t)nterms) return u128_to_seq((unsigned __int128)terms[nterms - 1]);
    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
