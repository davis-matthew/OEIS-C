/*
 * A003613  Number of symmetries in planted (1,4) trees on 3n-1 vertices.
 * https://oeis.org/A003613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P_1[n-1], where P_k[s] = sum over ternary rooted trees T
 *            with s internal nodes of |Aut(T)|^k, P_k[0] = 1, and P_k[s] =
 *            [x^(s-1)] (p1^3 + (6*2^k-3)*p1*p2 + (2-6*2^k+6^(k+1))*p3)/6 with
 *            p1 = P_k(x), p2 = P_{2k}(x^2), p3 = P_{3k}(x^3). Fixed-width
 *            builds compute this modulo three primes and c...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   is_prime_u64, mem, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

static void q3613_pk_big(idx_t k, idx_t N, seq_t *out)
{
    idx_t N2, N3, s, i;
    seq_t *q, *r, *sq;
    seq_t t2k, t6, c1, c2;
    out[0] = 1;
    if (N == 0) return;
    N2 = (N - 1) / 2;
    N3 = (N - 1) / 3;
    q = mem_new(seq_t, (size_t)(N2 + 1));
    r = mem_new(seq_t, (size_t)(N3 + 1));
    sq = mem_new(seq_t, (size_t)N);
    q3613_pk_big(2 * k, N2, q);
    q3613_pk_big(3 * k, N3, r);
    t2k = 1;
    for (i = 0; i < k; i++) t2k *= 2;
    t6 = 6;
    for (i = 0; i < k; i++) t6 *= 6;
    c1 = t2k * 6 - 3;
    c2 = t6 + 2 - t2k * 6;
    for (s = 1; s <= N; s++) {
        idx_t t = s - 1;
        seq_t acc = 0, cube = 0, pp = 0, v;
        for (i = 0; i <= t; i++) acc += out[i] * out[t - i];
        sq[t] = acc;
        for (i = 0; i <= t; i++) cube += out[i] * sq[t - i];
        for (i = 0; 2 * i <= t; i++) pp += q[i] * out[t - 2 * i];
        v = cube + c1 * pp;
        if (t % 3 == 0) v += c2 * r[t / 3];
        out[s] = v / 6;
    }
    mem_free(q);
    mem_free(r);
    mem_free(sq);
}

static void q3613_pk_mod(uint64_t k, idx_t N, uint64_t p, uint64_t *out)
{
    idx_t N2, N3, s, i;
    uint64_t *q, *r, *sq, t2k, c1, c2, inv6;
    out[0] = 1;
    if (N == 0) return;
    N2 = (N - 1) / 2;
    N3 = (N - 1) / 3;
    q = (uint64_t *)malloc((size_t)(N2 + 1) * sizeof(uint64_t));
    r = (uint64_t *)malloc((size_t)(N3 + 1) * sizeof(uint64_t));
    sq = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    q3613_pk_mod(2 * k, N2, p, q);
    q3613_pk_mod(3 * k, N3, p, r);
    t2k = powmod_u64(2, k, p);
    c1 = (mulmod(6, t2k, p) + p - 3 % p) % p;
    c2 = (2 + powmod_u64(6, k + 1, p)) % p;
    c2 = (c2 + p - mulmod(6, t2k, p)) % p;
    inv6 = powmod_u64(6, p - 2, p);
    for (s = 1; s <= N; s++) {
        idx_t t = s - 1;
        uint64_t acc = 0, cube = 0, pp = 0, tr, v;
        for (i = 0; i <= t; i++) acc = (acc + mulmod(out[i], out[t - i], p)) % p;
        sq[t] = acc;
        for (i = 0; i <= t; i++) cube = (cube + mulmod(out[i], sq[t - i], p)) % p;
        for (i = 0; 2 * i <= t; i++) pp = (pp + mulmod(q[i], out[t - 2 * i], p)) % p;
        tr = (t % 3 == 0) ? r[t / 3] : 0;
        v = (cube + mulmod(c1, pp, p)) % p;
        v = (v + mulmod(c2, tr, p)) % p;
        out[s] = mulmod(v, inv6, p);
    }
    free(q);
    free(r);
    free(sq);
}

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    seq_t x = 1;
    int i, fixed;
    for (i = 0; i < 130; i++) x += x;
    fixed = (x == 0);
    if (!fixed) {
        seq_t *out = mem_new(seq_t, (size_t)(m + 1));
        seq_t res;
        q3613_pk_big(1, m, out);
        res = out[m];
        mem_free(out);
        return res;
    } else {
        static uint64_t pr[3];
        static int have = 0;
        uint64_t rr[3], c2, c3, t;
        uint64_t *out;
        int j;
        if (n > 200) return 0; /* far beyond 128 bits; never checked */
        if (!have) {
            uint64_t c = ((uint64_t)1 << 62) - 1;
            j = 0;
            while (j < 3) {
                if (is_prime_u64(c)) pr[j++] = c;
                c -= 2;
            }
            have = 1;
        }
        out = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
        for (j = 0; j < 3; j++) {
            q3613_pk_mod(1, m, pr[j], out);
            rr[j] = out[m];
        }
        free(out);
        c2 = (rr[1] + pr[1] - rr[0] % pr[1]) % pr[1];
        c2 = mulmod(c2, powmod_u64(pr[0] % pr[1], pr[1] - 2, pr[1]), pr[1]);
        t = (rr[2] + pr[2] - rr[0] % pr[2]) % pr[2];
        t = mulmod(t, powmod_u64(pr[0] % pr[2], pr[2] - 2, pr[2]), pr[2]);
        t = (t + pr[2] - c2 % pr[2]) % pr[2];
        c3 = mulmod(t, powmod_u64(pr[1] % pr[2], pr[2] - 2, pr[2]), pr[2]);
        return (seq_t)rr[0] + (seq_t)pr[0] * ((seq_t)c2 + (seq_t)pr[1] * (seq_t)c3);
    }
}
