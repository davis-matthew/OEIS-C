/*
 * A077960  Trajectory of 18 under iteration of the map k -> A087712(k).
 * https://oeis.org/A077960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 18, a(n) = A087712(a(n-1)); iterate the map, where
 *            A087712(k) concatenates the prime ranks pi(p) of k's prime
 *            factors (factoring by trial division + Pollard rho, pi(p) by
 *            sieve or Lucy_Hedgehog)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 37, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 116 (2026-11-07)
 * Helpers:   build_primes_upto, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi_u64.h"

#define A077960_SMALL_LIM 4000000ULL

static uint64_t *A077960_sp = NULL;
static size_t A077960_sp_cnt = 0;

static void A077960_init_small(void) {
    if (!A077960_sp) build_primes_upto((idx_t)A077960_SMALL_LIM, &A077960_sp, &A077960_sp_cnt);
}

static uint64_t A077960_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A077960_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A077960_mulmod(r, b, m);
        b = A077960_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A077960_isprime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, r, s = 0;
    uint64_t d;
    if (n < 2) return false;
    for (i = 0; i < 12; ++i) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (i = 0; i < 12; ++i) {
        uint64_t x = A077960_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; ++r) {
            x = A077960_mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static uint64_t A077960_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A077960_rho(uint64_t n) {
    uint64_t c;
    if ((n & 1) == 0) return 2;
    for (c = 1;; ++c) {
        uint64_t x = 2, y = 2, d = 1, q = 1, ys = 2;
        uint64_t r = 1, k, i;
        do {
            x = y;
            for (i = 0; i < r; ++i) y = (A077960_mulmod(y, y, n) + c) % n;
            k = 0;
            do {
                ys = y;
                for (i = 0; i < 128 && i < r - k; ++i) {
                    y = (A077960_mulmod(y, y, n) + c) % n;
                    q = A077960_mulmod(q, x > y ? x - y : y - x, n);
                }
                d = A077960_gcd(q, n);
                k += 128;
            } while (k < r && d == 1);
            r <<= 1;
        } while (d == 1);
        if (d == n) {
            do {
                ys = (A077960_mulmod(ys, ys, n) + c) % n;
                d = A077960_gcd(x > ys ? x - ys : ys - x, n);
            } while (d == 1);
        }
        if (d != n) return d;
    }
}

static void A077960_factor_rec(uint64_t n, uint64_t *f, int *cnt) {
    uint64_t d;
    if (n == 1) return;
    if (A077960_isprime(n)) { f[(*cnt)++] = n; return; }
    d = A077960_rho(n);
    A077960_factor_rec(d, f, cnt);
    A077960_factor_rec(n / d, f, cnt);
}

static int A077960_factor(uint64_t n, uint64_t *f) {
    int cnt = 0, i, j;
    uint64_t p;
    for (p = 2; p < 1000 && p * p <= n; ++p) {
        while (n % p == 0) { f[cnt++] = p; n /= p; }
    }
    if (n > 1) A077960_factor_rec(n, f, &cnt);
    for (i = 1; i < cnt; ++i) {
        uint64_t v = f[i];
        j = i - 1;
        while (j >= 0 && f[j] > v) { f[j + 1] = f[j]; --j; }
        f[j + 1] = v;
    }
    return cnt;
}

static uint64_t A077960_lucy_pi(uint64_t N)
{
    return prime_pi_u64(N);   /* db/utils/prime_pi_u64.h */
}

static uint64_t A077960_rank_cache_p[4096];
static uint64_t A077960_rank_cache_r[4096];
static int A077960_rank_cache_n = 0;

static uint64_t A077960_rank(uint64_t p) {
    int i;
    uint64_t r;
    if (p <= A077960_SMALL_LIM) {
        size_t lo = 0, hi = A077960_sp_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (A077960_sp[mid] == p) return (uint64_t)mid + 1;
            if (A077960_sp[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        return 0;
    }
    for (i = 0; i < A077960_rank_cache_n; ++i)
        if (A077960_rank_cache_p[i] == p) return A077960_rank_cache_r[i];
    r = A077960_lucy_pi(p);
    if (A077960_rank_cache_n < 4096) {
        A077960_rank_cache_p[A077960_rank_cache_n] = p;
        A077960_rank_cache_r[A077960_rank_cache_n] = r;
        A077960_rank_cache_n++;
    }
    return r;
}

static uint64_t A077960_step(uint64_t x) {
    uint64_t f[128];
    int cnt, i;
    unsigned __int128 res = 0;
    if (x <= 1) return 1;
    cnt = A077960_factor(x, f);
    for (i = 0; i < cnt; ++i) {
        uint64_t r = A077960_rank(f[i]);
        unsigned __int128 pw = 10;
        uint64_t t = r;
        while (t >= 10) { pw *= 10; t /= 10; }
        res = res * pw + r;
    }
    return (uint64_t)res;
}

static uint64_t A077960_traj[512];
static idx_t A077960_len = 0;

seq_t a(idx_t n) {
    A077960_init_small();
    if (A077960_len == 0) { A077960_traj[0] = 18; A077960_len = 1; }
    while (A077960_len <= n && A077960_len < 512 && A077960_traj[A077960_len - 1] != 1) {
        A077960_traj[A077960_len] = A077960_step(A077960_traj[A077960_len - 1]);
        A077960_len++;
    }
    if (n < A077960_len) return (seq_t)A077960_traj[n];
    return (seq_t)A077960_traj[A077960_len - 1];
}
