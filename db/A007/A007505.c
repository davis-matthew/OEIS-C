/*
 * A007505  Primes of form 3*2^k - 1.
 * https://oeis.org/A007505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = 3*2^A002235(n) - 1; searches k = 0,1,2,... for primes
 *            3*2^k-1 (small-prime sieve, then the Lucas-Lehmer-Riesel test:
 *            u(0) = V_3(P) = P^3-3P, u(i+1) = u(i)^2-2 mod N, N prime iff
 *            u(k-2) == 0)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   jac_from_res, jac_odd, odd_part, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/jac_from_res.h"
#include "../utils/jac_odd.h"
#include "../utils/odd_part.h"

#define NSP 300

typedef unsigned __int128 u128;

static idx_t *found_k = NULL;
static idx_t nfound = 0, capfound = 0;
static idx_t next_k = 0;
static uint64_t sp[NSP];
static uint64_t spw[NSP];
static int nsp = 0;
static bool inited = false;
static seq_t bigT; /* 3*2^next_k, big build only */

static u128 mulmod128(u128 a, u128 b, u128 m) {
    if (m <= (u128)UINT64_MAX) return (a % m) * (b % m) % m;
    u128 r = 0;
    a %= m;
    while (b) {
        if (b & 1) { r += a; if (r >= m) r -= m; }
        a += a; if (a >= m) a -= m;
        b >>= 1;
    }
    return r;
}

static void init_state(void) {
    int i, j;
    static bool comp[2100];
    for (i = 2; i < 2100; i++) comp[i] = false;
    for (i = 2; i < 2100; i++) {
        if (comp[i]) continue;
        for (j = i * i; j < 2100; j += i) comp[j] = true;
        if (i > 3 && nsp < NSP) sp[nsp++] = (uint64_t)i;
    }
    for (i = 0; i < nsp; i++) spw[i] = 1;
    bigT = 3;
    inited = true;
}

static bool small_prime(uint64_t m) {
    uint64_t d;
    if (m < 2) return false;
    for (d = 2; d * d <= m; d++) if (m % d == 0) return false;
    return true;
}

static bool test_big(idx_t k) {
    seq_t N = bigT - 1;
    uint64_t P;
    for (P = 3; ; P++) {
        uint64_t m1 = odd_part(P - 2), m2 = odd_part(P + 2);
        uint64_t r1 = (uint64_t)(long long)(N % (seq_t)m1);
        uint64_t r2 = (uint64_t)(long long)(N % (seq_t)m2);
        int j1 = jac_from_res(m1, r1), j2 = jac_from_res(m2, r2);
        if (j1 == 0 || j2 == 0) return false;
        if (j1 == 1 && j2 == -1) break;
    }
    seq_t Ps = (seq_t)P;
    seq_t u = (Ps * Ps * Ps - (seq_t)3 * Ps) % N;
    idx_t i;
    for (i = 0; i + 2 < k; i++) u = (u * u + N - 2) % N;
    return u == 0;
}

static bool test_fixed(idx_t k) {
    u128 N = ((u128)3 << k) - 1;
    uint64_t P;
    for (P = 3; ; P++) {
        uint64_t m1 = odd_part(P - 2), m2 = odd_part(P + 2);
        int j1 = jac_from_res(m1, (uint64_t)(N % m1));
        int j2 = jac_from_res(m2, (uint64_t)(N % m2));
        if (j1 == 0 || j2 == 0) return false;
        if (j1 == 1 && j2 == -1) break;
    }
    u128 Pm = (u128)P % N;
    u128 u = mulmod128(mulmod128(Pm, Pm, N), Pm, N);
    u128 t3 = (3 * Pm) % N;
    u = (u + N - t3) % N;
    idx_t i;
    for (i = 0; i + 2 < k; i++) {
        u = mulmod128(u, u, N);
        u = (u + N - 2) % N;
    }
    return u == 0;
}

seq_t a(idx_t n) {
    bool big = seq_is_big() != 0;
    int i;
    if (!inited) init_state();
    while (nfound < n) {
        idx_t k = next_k;
        bool prime;
        if (!big && k > 125) break;
        if (k < 12) {
            prime = small_prime(((uint64_t)3 << k) - 1);
        } else {
            bool cand = true;
            for (i = 0; i < nsp; i++)
                if ((3 * spw[i]) % sp[i] == 1) { cand = false; break; }
            prime = cand && (big ? test_big(k) : test_fixed(k));
        }
        if (prime) {
            if (nfound == capfound) {
                capfound = capfound ? 2 * capfound : 64;
                found_k = (idx_t *)realloc(found_k, capfound * sizeof(idx_t));
            }
            found_k[nfound++] = k;
        }
        for (i = 0; i < nsp; i++) spw[i] = (spw[i] * 2) % sp[i];
        if (big) bigT += bigT;
        next_k++;
    }
    if (nfound < n) return (seq_t)0; /* does not fit in seq_t */
    idx_t k = found_k[n - 1];
    if (big) {
        seq_t v = 3;
        idx_t j;
        for (j = 0; j < k; j++) v += v;
        return v - 1;
    }
    return u128_to_seq(((u128)3 << k) - 1);
}
