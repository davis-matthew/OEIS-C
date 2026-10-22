/*
 * A003614  Symmetries in unrooted (1,4) trees on 3n-1 vertices.
 * https://oeis.org/A003614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n >= 2, a(n) = sum of |Aut(T)| over (1,4)-trees T
 *            with i = n-1 internal nodes. Let Q_k(m) be the sum of |Aut|^k
 *            over planted trees with m internal nodes, with Q_k(0) = 1 (a
 *            leaf). Let A_k(x) = Sum_m Q_k(m) x^m. Then Q_k(m) = [x^(m-1)] (
 *            (A_k^3 - 3 A_k A_2k(x^2) + 2 A_3k(x^3))/6 + 2^...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   a3614_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3614_find.h"

typedef unsigned __int128 a3614_u128;

static uint64_t a3614_mul(uint64_t x, uint64_t y, uint64_t p)
{
    return (uint64_t)((a3614_u128)x * y % p);
}

static uint64_t a3614_pw(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = a3614_mul(r, b, p);
        b = a3614_mul(b, b, p);
        e >>= 1;
    }
    return r;
}

static int a3614_isprime(uint64_t m)
{
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d, x;
    int s, i, r;
    if (m < 2) return 0;
    for (i = 0; i < 12; i++)
        if (m % bs[i] == 0) return m == bs[i];
    d = m - 1; s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        x = a3614_pw(bs[i], d, m);
        if (x == 1 || x == m - 1) continue;
        for (r = 1; r < s; r++) {
            x = a3614_mul(x, x, m);
            if (x == m - 1) break;
        }
        if (r >= s) return 0;
    }
    return 1;
}

#define A3614_MAXP 4096
static uint64_t a3614_pr[A3614_MAXP];
static int a3614_np = 0;
static uint64_t a3614_next = ((uint64_t)1 << 61) - 1;

static uint64_t a3614_prime(int j)
{
    while (a3614_np <= j) {
        while (!a3614_isprime(a3614_next)) a3614_next -= 2;
        a3614_pr[a3614_np++] = a3614_next;
        a3614_next -= 2;
    }
    return a3614_pr[j];
}

/* sum_{j=lo..hi} x[j] * y[t - step*j] mod p */
static uint64_t a3614_dot(const uint64_t *x, const uint64_t *y, uint64_t t,
                          uint64_t step, uint64_t lo, uint64_t hi, uint64_t p)
{
    a3614_u128 acc = 0;
    uint64_t j;
    int c = 0;
    for (j = lo; j <= hi; j++) {
        acc += (a3614_u128)x[j] * y[t - step * j];
        if (++c == 32) { acc %= p; c = 0; }
    }
    return (uint64_t)(acc % p);
}

static uint64_t a3614_mod(uint64_t i, uint64_t p)
{
    static const uint64_t one[1] = {1};
    uint64_t K = i < 4 ? 4 : i;
    uint64_t ks[1024];
    uint64_t *Q[1024];
    uint64_t a, b, inv6, inv24, inv2, res;
    int nk = 0, x, y;

    for (a = 1; a <= K && nk < 1024; a *= 2)
        for (b = a; b <= K && nk < 1024; b *= 3) ks[nk++] = b;
    for (x = 1; x < nk; x++) {
        uint64_t v = ks[x];
        y = x - 1;
        while (y >= 0 && ks[y] < v) { ks[y + 1] = ks[y]; y--; }
        ks[y + 1] = v;
    }
    inv6 = a3614_pw(6, p - 2, p);
    inv24 = a3614_pw(24, p - 2, p);
    inv2 = a3614_pw(2, p - 2, p);

    for (x = 0; x < nk; x++) {
        uint64_t k = ks[x];
        uint64_t Lk = i / (k < 2 ? 2 : k), m;
        int i2 = a3614_find(ks, nk, 2 * k), i3 = a3614_find(ks, nk, 3 * k);
        const uint64_t *Q2 = i2 >= 0 ? Q[i2] : one;
        const uint64_t *Q3 = i3 >= 0 ? Q[i3] : one;
        uint64_t pk2 = a3614_pw(2, k, p), pk6 = a3614_pw(6, k, p);
        uint64_t *A = (uint64_t *)malloc((size_t)(Lk + 1) * sizeof(uint64_t));
        uint64_t *A2 = (uint64_t *)malloc((size_t)(Lk + 1) * sizeof(uint64_t));
        A[0] = 1;
        for (m = 1; m <= Lk; m++) {
            uint64_t t = m - 1, A3, AB, Cc, pos, neg, e3, q;
            A2[t] = a3614_dot(A, A, t, 1, 0, t, p);
            A3 = a3614_dot(A, A2, t, 1, 0, t, p);
            AB = a3614_dot(Q2, A, t, 2, 0, t / 2, p);
            Cc = (t % 3 == 0) ? Q3[t / 3] : 0;
            pos = (A3 + 2 * Cc) % p;
            neg = a3614_mul(3, AB, p);
            e3 = a3614_mul((pos + p - neg) % p, inv6, p);
            q = (e3 + a3614_mul(pk2, (AB + p - Cc) % p, p)) % p;
            q = (q + a3614_mul(pk6, Cc, p)) % p;
            A[m] = q;
        }
        free(A2);
        Q[x] = A;
    }

    {
        const uint64_t *Q1 = Q[a3614_find(ks, nk, 1)];
        const uint64_t *Q2 = Q[a3614_find(ks, nk, 2)];
        const uint64_t *Q3 = Q[a3614_find(ks, nk, 3)];
        const uint64_t *Q4 = Q[a3614_find(ks, nk, 4)];
        uint64_t t = i - 1, hp = (i - 1) / 2, u, s;
        uint64_t A4, A2B, B2, AC, D, V;
        uint64_t *Ar = (uint64_t *)malloc((size_t)(t + 1) * sizeof(uint64_t));
        uint64_t *A2f = (uint64_t *)malloc((size_t)(t + 1) * sizeof(uint64_t));
        for (s = 0; s <= t; s++) Ar[s] = (s <= hp) ? Q1[s] : 0;
        for (u = 0; u <= t; u++) {
            uint64_t lo = u > hp ? u - hp : 0, hi = u < hp ? u : hp;
            A2f[u] = a3614_dot(Ar, Ar, u, 1, lo, hi, p);
        }
        A4 = a3614_dot(A2f, A2f, t, 1, 0, t, p);
        A2B = a3614_dot(Q2, A2f, t, 2, 0, t / 2, p);
        B2 = (t % 2 == 0) ? a3614_dot(Q2, Q2, t / 2, 1, 0, t / 2, p) : 0;
        AC = a3614_dot(Q3, Ar, t, 3, 0, t / 3, p);
        D = (t % 4 == 0) ? Q4[t / 4] : 0;
        V = A4;
        V = (V + a3614_mul(18, A2B, p)) % p;
        V = (V + a3614_mul(27, B2, p)) % p;
        V = (V + a3614_mul(104, AC, p)) % p;
        V = (V + a3614_mul(426, D, p)) % p;
        res = a3614_mul(V, inv24, p);
        if (i % 2 == 0) {
            uint64_t h = i / 2;
            uint64_t E = (a3614_mul(Q1[h], Q1[h], p) + a3614_mul(3, Q2[h], p)) % p;
            res = (res + a3614_mul(E, inv2, p)) % p;
        }
        free(Ar);
        free(A2f);
    }
    for (x = 0; x < nk; x++) free(Q[x]);
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t d[A3614_MAXP];
    seq_t val = 0, prod = 1, w = 1;
    int bits, big, cap, j, l, zeros = 0;

    if (n <= 1) return (seq_t)2;
    for (bits = 0; bits < 256; bits++) {
        if (w == 0) break;
        w = w + w;
    }
    big = bits >= 256;
    cap = big ? A3614_MAXP : bits / 60 + 1;

    for (j = 0; j < cap; j++) {
        uint64_t p = a3614_prime(j);
        uint64_t x = a3614_mod((uint64_t)(n - 1), p);
        for (l = 0; l < j; l++) {
            x = (x + p - d[l] % p) % p;
            x = a3614_mul(x, a3614_pw(a3614_pr[l] % p, p - 2, p), p);
        }
        d[j] = x;
        val += prod * (seq_t)x;
        prod *= (seq_t)p;
        if (big) {
            if (j > 0 && x == 0) {
                if (++zeros >= 2) break;
            } else {
                zeros = 0;
            }
        }
    }
    return val;
}
