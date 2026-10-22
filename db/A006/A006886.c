/*
 * A006886  Kaprekar numbers: positive numbers n such that n = q+r and n^2 =
 *          q*10^m+r, for some m >= 1, q >= 0 and 0 <= r < 10^m, with n !=
 *          10^a, a >= 1.
 * https://oeis.org/A006886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base, easy
 * Formula:   no closed form; a(n) is the n-th smallest element of the union
 *            over m >= 1 of { x : 1 <= x <= 10^m-1, (10^m-1) | x*(x-1) }. For
 *            each m and each unitary divisor d of 10^m-1, the element is x =
 *            d*(d^-1 mod (10^m-1)/d) (CRT: x == 0 mod d, x == 1 mod
 *            (10^m-1)/d). 10^m-1 is factored incrementally: pri...
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 110, u64 n <= 810,
 *            u128 n <= 9396, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 kap_u128;

#define KAP_MAXM 38
#define KAP_MAXP 1024

static kap_u128 kap_primes[KAP_MAXP];
static int kap_pord[KAP_MAXP];
static int kap_np = 0;

static kap_u128 *kap_list = NULL;
static size_t kap_count = 0;
static bool kap_built = false;

static kap_u128 kap_mulmod(kap_u128 a, kap_u128 b, kap_u128 m) {
    kap_u128 r = 0;
    a %= m;
    while (b) {
        if (b & 1) { r += a; if (r >= m) r -= m; }
        b >>= 1;
        if (b) { a += a; if (a >= m) a -= m; }
    }
    return r;
}

static kap_u128 kap_powmod(kap_u128 b, kap_u128 e, kap_u128 m) {
    kap_u128 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = kap_mulmod(r, b, m);
        e >>= 1;
        if (e) b = kap_mulmod(b, b, m);
    }
    return r;
}

static bool kap_isprime(kap_u128 n) {
    static const unsigned bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                     41, 43, 47, 53, 59, 61, 67, 71};
    int nb = (int)(sizeof(bases) / sizeof(bases[0]));
    int i, s = 0, r;
    kap_u128 d;
    if (n < 2) return false;
    for (i = 0; i < nb; i++)
        if (n % bases[i] == 0) return n == bases[i];
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < nb; i++) {
        kap_u128 x = kap_powmod(bases[i], d, n);
        bool ok = false;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = kap_mulmod(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static kap_u128 kap_gcd(kap_u128 a, kap_u128 b) {
    while (b) { kap_u128 t = a % b; a = b; b = t; }
    return a;
}

static kap_u128 kap_rho(kap_u128 n) {
    kap_u128 c;
    if ((n & 1) == 0) return 2;
    for (c = 1;; c++) {
        kap_u128 x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (kap_mulmod(x, x, n) + c) % n;
            y = (kap_mulmod(y, y, n) + c) % n;
            y = (kap_mulmod(y, y, n) + c) % n;
            d = kap_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void kap_add_prime(kap_u128 p, int m) {
    int i;
    for (i = 0; i < kap_np; i++)
        if (kap_primes[i] == p) return;
    if (kap_np < KAP_MAXP) {
        kap_primes[kap_np] = p;
        kap_pord[kap_np] = m;
        kap_np++;
    }
}

static void kap_factor(kap_u128 n, int m) {
    kap_u128 d;
    if (n == 1) return;
    if (kap_isprime(n)) { kap_add_prime(n, m); return; }
    d = kap_rho(n);
    kap_factor(d, m);
    kap_factor(n / d, m);
}

/* inverse of a modulo mod (gcd(a,mod)=1), via extended Euclid on magnitudes */
static kap_u128 kap_modinv(kap_u128 a, kap_u128 mod) {
    kap_u128 a0 = mod, a1 = a, s0 = 0, s1 = 1;
    int k = 0;
    while (a1 != 0) {
        kap_u128 q = a0 / a1, t;
        t = a0 - q * a1; a0 = a1; a1 = t;
        t = s0 + q * s1; s0 = s1; s1 = t;
        k++;
    }
    return (k & 1) ? s0 : mod - s0;
}

static int kap_cmp(const void *pa, const void *pb) {
    kap_u128 x = *(const kap_u128 *)pa, y = *(const kap_u128 *)pb;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static void kap_build(void) {
    size_t cap = 1024, nc = 0, i, j;
    kap_u128 *cand = (kap_u128 *)malloc(cap * sizeof(kap_u128));
    int m;
    for (m = 1; m <= KAP_MAXM; m++) {
        kap_u128 B = 1, C, p, step;
        kap_u128 pps[64];
        int k = 0, t;
        unsigned long mask, nmask;
        for (t = 0; t < m; t++) B *= 10;
        B -= 1;
        C = B;
        for (t = 0; t < kap_np; t++)
            if (m % kap_pord[t] == 0)
                while (C % kap_primes[t] == 0) C /= kap_primes[t];
        step = (m % 2 == 0) ? (kap_u128)m : (kap_u128)(2 * m);
        for (p = step + 1; p <= ((kap_u128)1 << 22) && p * p <= C; p += step) {
            if (C % p == 0) {
                kap_add_prime(p, m);
                while (C % p == 0) C /= p;
            }
        }
        if (C > 1) kap_factor(C, m);
        for (t = 0; t < kap_np; t++) {
            if (m % kap_pord[t] == 0) {
                kap_u128 X = B, pp = 1;
                while (X % kap_primes[t] == 0) { X /= kap_primes[t]; pp *= kap_primes[t]; }
                if (k < 64) pps[k++] = pp;
            }
        }
        nmask = 1UL << k;
        for (mask = 0; mask < nmask; mask++) {
            kap_u128 D = 1, E, inv;
            for (t = 0; t < k; t++)
                if ((mask >> t) & 1UL) D *= pps[t];
            E = B / D;
            inv = kap_modinv(D % E, E);
            if (nc == cap) {
                cap *= 2;
                cand = (kap_u128 *)realloc(cand, cap * sizeof(kap_u128));
            }
            cand[nc++] = D * inv;
        }
    }
    qsort(cand, nc, sizeof(kap_u128), kap_cmp);
    j = 0;
    for (i = 0; i < nc; i++)
        if (j == 0 || cand[i] != cand[j - 1]) cand[j++] = cand[i];
    kap_list = cand;
    kap_count = j;
    kap_built = true;
}

seq_t a(idx_t n) {
    if (!kap_built) kap_build();
    if (n < 1 || (size_t)(n - 1) >= kap_count) return (seq_t)0;
    return u128_to_seq(kap_list[n - 1]);
}
