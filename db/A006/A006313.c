/*
 * A006313  Numbers k such that k^16 + 1 is prime.
 * https://oeis.org/A006313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that k^16 + 1 is prime. Search: k = 1, 2
 *            handled directly. Odd k > 1 make k^16 + 1 even, so only even k
 *            are tested. Candidates are sieved by primes p == 1 (mod 32)
 *            using the 16 roots of x^16 == -1 (mod p). Survivors get a strong
 *            probable-prime test to bases 3, 5, 7 in mult...
 * Range:     u8 n <= 12, u16 n <= 1497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mp_eq, mp_ge, pm32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mp_eq.h"
#include "../utils/mp_ge.h"
#include "../utils/pm32.h"

#define ML 10
#define SIEVE_B (1u << 23)
#define BLK (1u << 16)

typedef unsigned __int128 u128_t;

static void mp_sub(uint64_t *x, const uint64_t *y, int L) {
    uint64_t br = 0;
    for (int i = 0; i < L; i++) {
        uint64_t xi = x[i], yi = y[i];
        uint64_t t = xi - yi;
        uint64_t nb = (xi < yi);
        uint64_t t2 = t - br;
        nb |= (t < br);
        x[i] = t2;
        br = nb;
    }
}

static void mp_addmod(uint64_t *x, const uint64_t *y, const uint64_t *N, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        u128_t s = (u128_t)x[i] + y[i] + c;
        x[i] = (uint64_t)s;
        c = (uint64_t)(s >> 64);
    }
    if (c || mp_ge(x, N, L)) mp_sub(x, N, L);
}

static void mont_mul(uint64_t *r, const uint64_t *x, const uint64_t *y,
                     const uint64_t *N, uint64_t ninv, int L) {
    uint64_t t[ML + 2];
    for (int i = 0; i < L + 2; i++) t[i] = 0;
    for (int i = 0; i < L; i++) {
        u128_t c = 0;
        uint64_t yi = y[i];
        for (int j = 0; j < L; j++) {
            c += (u128_t)x[j] * yi + t[j];
            t[j] = (uint64_t)c;
            c >>= 64;
        }
        c += t[L];
        t[L] = (uint64_t)c;
        t[L + 1] = (uint64_t)(c >> 64);
        uint64_t m = t[0] * ninv;
        c = (u128_t)m * N[0] + t[0];
        c >>= 64;
        for (int j = 1; j < L; j++) {
            c += (u128_t)m * N[j] + t[j];
            t[j - 1] = (uint64_t)c;
            c >>= 64;
        }
        c += t[L];
        t[L - 1] = (uint64_t)c;
        c >>= 64;
        t[L] = t[L + 1] + (uint64_t)c;
    }
    if (t[L] || mp_ge(t, N, L)) mp_sub(t, N, L);
    for (int i = 0; i < L; i++) r[i] = t[i];
}

static bool sprp(const uint64_t *N, int L, uint64_t base) {
    uint64_t inv = N[0];
    for (int i = 0; i < 6; i++) inv *= (uint64_t)2 - N[0] * inv;
    uint64_t ninv = (uint64_t)0 - inv;

    uint64_t one[ML], mone[ML], d[ML], x[ML], tab[16][ML];
    for (int i = 0; i < L; i++) one[i] = 0;
    one[0] = 1;
    for (int it = 0; it < 64 * L; it++) {
        uint64_t carry = one[L - 1] >> 63;
        for (int i = L - 1; i > 0; i--) one[i] = (one[i] << 1) | (one[i - 1] >> 63);
        one[0] <<= 1;
        if (carry || mp_ge(one, N, L)) mp_sub(one, N, L);
    }
    for (int i = 0; i < L; i++) { tab[0][i] = one[i]; tab[1][i] = 0; }
    for (uint64_t q = 0; q < base; q++) mp_addmod(tab[1], one, N, L);
    for (int i = 2; i < 16; i++) mont_mul(tab[i], tab[i - 1], tab[1], N, ninv, L);

    for (int i = 0; i < L; i++) mone[i] = N[i];
    mp_sub(mone, one, L);

    for (int i = 0; i < L; i++) d[i] = N[i];
    d[0] -= 1;
    int s = 0;
    while ((d[0] & 1) == 0) {
        for (int i = 0; i < L - 1; i++) d[i] = (d[i] >> 1) | (d[i + 1] << 63);
        d[L - 1] >>= 1;
        s++;
    }
    int hi = L - 1;
    while (hi > 0 && d[hi] == 0) hi--;
    int bits = hi * 64;
    {
        uint64_t top = d[hi];
        while (top) { bits++; top >>= 1; }
    }
    int nibs = (bits + 3) / 4;
    bool started = false;
    for (int q = nibs - 1; q >= 0; q--) {
        int pos = 4 * q;
        int nib = (int)((d[pos / 64] >> (pos % 64)) & 15);
        if (started) {
            for (int r = 0; r < 4; r++) mont_mul(x, x, x, N, ninv, L);
            if (nib) mont_mul(x, x, tab[nib], N, ninv, L);
        } else if (nib) {
            for (int i = 0; i < L; i++) x[i] = tab[nib][i];
            started = true;
        }
    }
    if (!started) for (int i = 0; i < L; i++) x[i] = one[i];
    if (mp_eq(x, one, L) || mp_eq(x, mone, L)) return true;
    for (int r = 1; r < s; r++) {
        mont_mul(x, x, x, N, ninv, L);
        if (mp_eq(x, mone, L)) return true;
        if (mp_eq(x, one, L)) return false;
    }
    return false;
}

static bool test_k(uint64_t k) {
    uint64_t N[ML];
    int L = 1;
    N[0] = 1;
    for (int e = 0; e < 16; e++) {
        uint64_t c = 0;
        for (int i = 0; i < L; i++) {
            u128_t p = (u128_t)N[i] * k + c;
            N[i] = (uint64_t)p;
            c = (uint64_t)(p >> 64);
        }
        if (c) N[L++] = c;
    }
    N[0] += 1; /* k even: k^16 is even, no carry */
    return sprp(N, L, 3) && sprp(N, L, 5) && sprp(N, L, 7);
}

static uint32_t *SP = NULL;
static uint32_t *SR = NULL;
static size_t nsp = 0;

static void sieve_setup(void) {
    unsigned char *comp = (unsigned char *)calloc((size_t)SIEVE_B + 1, 1);
    size_t cntp = 0;
    for (uint64_t i = 2; i * i <= SIEVE_B; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= SIEVE_B; j += i) comp[j] = 1;
    for (uint64_t p = 97; p <= SIEVE_B; p += 32)
        if (!comp[p]) cntp++;
    SP = (uint32_t *)malloc((cntp + 1) * sizeof(uint32_t));
    SR = (uint32_t *)malloc((cntp + 1) * 16 * sizeof(uint32_t));
    nsp = 0;
    for (uint64_t p = 97; p <= SIEVE_B; p += 32) {
        if (comp[p]) continue;
        uint64_t r = 0;
        for (uint64_t g = 2; g < p; g++) {
            r = pm32(g, (p - 1) / 32, p);
            if (pm32(r, 16, p) == p - 1) break;
        }
        uint64_t r2 = r * r % p, cur = r;
        SP[nsp] = (uint32_t)p;
        for (int j = 0; j < 16; j++) {
            SR[nsp * 16 + j] = (uint32_t)cur;
            cur = cur * r2 % p;
        }
        nsp++;
    }
    free(comp);
}

static uint64_t *T = NULL;
static idx_t tcnt = 0, tcap = 0;
static uint64_t next_lo = 3;

static void push_term(uint64_t k) {
    if (tcnt == tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        T = (uint64_t *)realloc(T, tcap * sizeof(uint64_t));
    }
    T[tcnt++] = k;
}

static void process_block(uint64_t lo) {
    unsigned char *mark = (unsigned char *)calloc(BLK, 1);
    for (size_t q = 0; q < nsp; q++) {
        uint64_t p = SP[q];
        uint64_t lm = lo % p;
        for (int j = 0; j < 16; j++) {
            uint64_t r = SR[q * 16 + j];
            uint64_t st = (r + p - lm) % p;
            for (uint64_t i = st; i < BLK; i += p) mark[i] = 1;
        }
    }
    for (uint64_t i = 0; i < BLK; i++) {
        uint64_t k = lo + i;
        if (k & 1) continue;
        if (mark[i]) continue;
        if (test_k(k)) push_term(k);
    }
    free(mark);
}

seq_t a(idx_t n) {
    static bool init = false;
    if (!init) {
        sieve_setup();
        push_term(1);
        push_term(2);
        next_lo = 3;
        init = true;
    }
    while (tcnt < n) {
        process_block(next_lo);
        next_lo += BLK;
    }
    return (seq_t)T[n - 1];
}
