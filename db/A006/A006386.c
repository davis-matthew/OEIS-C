/*
 * A006386  Number of sensed genus 1 maps with n edges.
 * https://oeis.org/A006386
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = (1/(2n)) * ( Sum_{d|n} J_2(d)*R(n/d) + Sum_{s=0..4, s==n
 *            mod 2} binomial(n,s)*A000168((n-s)/2)*binomial((n-s)/2+2,4-s) +
 *            [3|n] 2*A000168(n/3)*binomial(n/3+2,3) + [2|n] 2*Q4(n/2) + [3|n]
 *            2*Q6(n/3) ), where J_2 is Jordan's totient, R(k) = A006300(k)
 *            (rooted toroidal maps) with R(k) = 3^(k-1)...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, jordan2, mm_ex, release, sbin, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/jordan2.h"
#include "../utils/mm_ex.h"
#include "../utils/sbin.h"

/* ---------- modular path ---------- */
static uint64_t mulm(uint64_t a, uint64_t b, uint64_t p) {
    return (uint64_t)((unsigned __int128)a * b % p);
}
static uint64_t powm(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = mulm(r, b, p);
        b = mulm(b, b, p);
        e >>= 1;
    }
    return r;
}
static uint64_t invm(uint64_t a, uint64_t p) { return powm(a % p, p - 2, p); }

static uint64_t bm(idx_t D, int s, uint64_t p) {
    uint64_t num = 1, den = 1;
    int i;
    if ((idx_t)s > D) return 0;
    for (i = 0; i < s; i++) {
        num = mulm(num, (D - (idx_t)i) % p, p);
        den = mulm(den, (uint64_t)(i + 1), p);
    }
    return mulm(num, invm(den, p), p);
}

static uint64_t mm_mod(const uint64_t *c3, idx_t e, uint64_t p) {
    return mulm((2 * c3[e]) % p, invm((e + 2) % p, p), p);
}

static uint64_t a_mod(idx_t n, uint64_t p) {
    uint64_t *c3 = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    uint64_t *R = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    uint64_t S = 0, t;
    idx_t k, e, D, L;
    int s;
    c3[0] = 1;
    for (k = 1; k <= n; k++)
        c3[k] = mulm(mulm(c3[k - 1], (6 * (2 * k - 1)) % p, p), invm(k + 1, p), p);
    R[0] = R[1] = 0;
    for (k = 2; k <= n; k++) {
        uint64_t h = (c3[k - 1] + 2 * (p - c3[k - 2])) % p;
        R[k] = (h + mulm(8, R[k - 1], p) + mulm(48, R[k - 2], p)) % p;
    }
    for (L = 1; L <= n; L++)
        if (n % L == 0) S = (S + mulm(jordan2(L) % p, R[n / L], p)) % p;
    for (s = 0; s <= 4; s++)
        if ((idx_t)s <= n && (n - (idx_t)s) % 2 == 0) {
            e = (n - (idx_t)s) / 2;
            t = mulm(mulm(bm(n, s, p), mm_mod(c3, e, p), p), bm(e + 2, 4 - s, p), p);
            S = (S + t) % p;
        }
    if (n % 3 == 0) {
        e = n / 3;
        t = mulm(mulm(2, mm_mod(c3, e, p), p), bm(e + 2, 3, p), p);
        S = (S + t) % p;
    }
    if (n % 2 == 0) {
        D = n / 2;
        if (D % 2 == 0) {
            e = D / 2;
            t = mulm(mulm(2, mm_mod(c3, e, p), p), mulm((e + 2) % p, bm(e + 1, 2, p), p), p);
        } else {
            e = (D - 1) / 2;
            t = mulm(mulm((2 * D) % p, mm_mod(c3, e, p), p), bm(e + 2, 2, p), p);
        }
        S = (S + t) % p;
    }
    if (n % 3 == 0) {
        D = n / 3;
        if (D % 2 == 0) {
            e = D / 2;
            t = mulm(mulm(2, mm_mod(c3, e, p), p), ((e + 2) * (e + 1) * e) % p, p);
        } else {
            e = (D - 1) / 2;
            t = mulm(mulm((2 * D) % p, mm_mod(c3, e, p), p), ((e + 2) * (e + 1)) % p, p);
        }
        S = (S + t) % p;
    }
    free(c3);
    free(R);
    return mulm(S, invm((2 * n) % p, p), p);
}

static seq_t a_exact(idx_t n) {
    seq_t *c3 = alloc_zero(n + 1);
    seq_t *R = alloc_zero(n + 1);
    seq_t S = 0;
    idx_t k, e, D, L;
    int s;
    c3[0] = 1;
    for (k = 1; k <= n; k++) c3[k] = c3[k - 1] * (6 * (2 * k - 1)) / (k + 1);
    R[0] = 0;
    R[1] = 0;
    for (k = 2; k <= n; k++)
        R[k] = c3[k - 1] - c3[k - 2] * 2 + R[k - 1] * 8 + R[k - 2] * 48;
    for (L = 1; L <= n; L++)
        if (n % L == 0) S += R[n / L] * (idx_t)jordan2(L);
    for (s = 0; s <= 4; s++)
        if ((idx_t)s <= n && (n - (idx_t)s) % 2 == 0) {
            e = (n - (idx_t)s) / 2;
            S += sbin(n, s) * mm_ex(c3, e) * sbin(e + 2, 4 - s);
        }
    if (n % 3 == 0) {
        e = n / 3;
        S += mm_ex(c3, e) * 2 * sbin(e + 2, 3);
    }
    if (n % 2 == 0) {
        D = n / 2;
        if (D % 2 == 0) {
            e = D / 2;
            S += mm_ex(c3, e) * 2 * (e + 2) * sbin(e + 1, 2);
        } else {
            e = (D - 1) / 2;
            S += mm_ex(c3, e) * (2 * D) * sbin(e + 2, 2);
        }
    }
    if (n % 3 == 0) {
        D = n / 3;
        if (D % 2 == 0) {
            e = D / 2;
            S += mm_ex(c3, e) * 2 * ((e + 2) * (e + 1) * e);
        } else {
            e = (D - 1) / 2;
            S += mm_ex(c3, e) * (2 * D) * ((e + 2) * (e + 1));
        }
    }
    release(c3, n + 1);
    release(R, n + 1);
    return S / (2 * n);
}

seq_t a(idx_t n) {
    static const uint64_t P[4] = {2305843009213693951ULL, 1000000007ULL,
                                  1000000009ULL, 998244353ULL};
    uint64_t d[4];
    seq_t v;
    int i, j;
    if (!seq_is_fixed_width()) return a_exact(n);
    for (i = 0; i < 4; i++) {
        uint64_t x = a_mod(n, P[i]);
        for (j = 0; j < i; j++) {
            uint64_t dj = d[j] % P[i];
            x = (x + P[i] - dj) % P[i];
            x = mulm(x, invm(P[j] % P[i], P[i]), P[i]);
        }
        d[i] = x;
    }
    v = (seq_t)(idx_t)d[3];
    v = v * (idx_t)P[2] + (idx_t)d[2];
    v = v * (idx_t)P[1] + (idx_t)d[1];
    v = v * (idx_t)P[0] + (idx_t)d[0];
    return v;
}
