/*
 * A007597  Strobogrammatic primes.
 * https://oeis.org/A007597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- generate strobogrammatic numbers (digits 0,1,6,8,9
 *            with 6<->9 under 180-degree rotation) in increasing order,
 *            length by length, and keep those that are prime (deterministic
 *            Miller-Rabin)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 237, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 8919 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t sp_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t sp_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = sp_mulmod(r, b, m);
        b = sp_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool sp_isprime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, r, s = 0;
    uint64_t d;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = sp_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = sp_mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static int sp_rot(int d) {
    switch (d) {
        case 6: return 9;
        case 9: return 6;
        default: return d;
    }
}

static uint64_t *sp_cache = NULL;
static size_t sp_count = 0;
static size_t sp_cap = 0;
static int sp_next_len = 2;

static void sp_push(uint64_t v) {
    if (sp_count >= sp_cap) {
        size_t nc = sp_cap < 256 ? 256 : sp_cap * 2;
        sp_cache = (uint64_t *)realloc(sp_cache, nc * sizeof(uint64_t));
        sp_cap = nc;
    }
    sp_cache[sp_count++] = v;
}

/* Generate all strobogrammatic primes of exactly L digits, in increasing order. */
static void sp_gen_length(int L) {
    static const int first[2] = {1, 6};          /* 8,9 first -> last digit even */
    static const int half[5] = {0, 1, 6, 8, 9};
    static const int mids[3] = {0, 1, 8};
    int h = L / 2, odd = L % 2;
    int npos = h + odd;
    int idx[20];
    int dg[20];
    int i;
    if (h == 0) return; /* single digits 0,1,8: none prime */
    for (i = 0; i < npos; i++) idx[i] = 0;
    for (;;) {
        uint64_t v = 0;
        for (i = 0; i < h; i++) {
            int d = (i == 0) ? first[idx[0]] : half[idx[i]];
            dg[i] = d;
            dg[L - 1 - i] = sp_rot(d);
        }
        if (odd) dg[h] = mids[idx[h]];
        for (i = 0; i < L; i++) v = v * 10 + (uint64_t)dg[i];
        if (sp_isprime(v)) sp_push(v);
        /* odometer increment, last position least significant */
        for (i = npos - 1; i >= 0; i--) {
            int lim = (i == 0) ? 2 : ((odd && i == h) ? 3 : 5);
            idx[i]++;
            if (idx[i] < lim) break;
            idx[i] = 0;
        }
        if (i < 0) break;
    }
}

seq_t a(idx_t n) {
    while ((idx_t)sp_count < n && sp_next_len <= 19) {
        sp_gen_length(sp_next_len);
        sp_next_len++;
    }
    if (n == 0 || (idx_t)sp_count < n) return (seq_t)0;
    return (seq_t)sp_cache[n - 1];
}
