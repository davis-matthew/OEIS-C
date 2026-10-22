/*
 * A002830  Number of 3-edge-colored trivalent graphs with 2n nodes.
 * https://oeis.org/A002830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. exp(Sum_{k>=1} F(x^k)/k), F = g.f. of A002831; equivalently
 *            (Burnside) a(n) = (1/(2n)!) * Sum_{s in S_{2n}} I(s)^3, where
 *            I(s) is the number of fixed-point-free involutions commuting
 *            with s. Grouping by cycle type gives Sum_n a(n) z^n = Prod_{k
 *            odd} Phi(k z^k) * Prod_{k=2l} G_k(z^l), where P...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   big_mod, conv_at, log_series, pr_isprime, pw_mod, solve_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_mod.h"
#include "../utils/conv_at.h"
#include "../utils/log_series.h"
#include "../utils/pr_isprime.h"
#include "../utils/pw_mod.h"
#include "../utils/solve_mod.h"

static uint32_t *pr_list = NULL;
static size_t pr_cnt = 0, pr_cap = 0;

static uint64_t pr_get(size_t i) {
    while (pr_cnt <= i) {
        uint64_t c = pr_cnt ? (uint64_t)pr_list[pr_cnt - 1] - 2 : ((uint64_t)1 << 30) - 1;
        while (!pr_isprime(c)) c -= 2;
        if (pr_cnt == pr_cap) {
            size_t nc = pr_cap ? pr_cap * 2 : 64;
            uint32_t *np = (uint32_t *)malloc(nc * sizeof(uint32_t));
            if (pr_cnt) memcpy(np, pr_list, pr_cnt * sizeof(uint32_t));
            free(pr_list);
            pr_list = np;
            pr_cap = nc;
        }
        pr_list[pr_cnt++] = (uint32_t)c;
    }
    return pr_list[i];
}

seq_t a(idx_t n) {
    double mm = 1.0;
    long e = 0;
    idx_t i;
    long bits;
    size_t np, q, LEN, t;
    uint32_t *R, *Q;
    seq_t res = 0;

    if (n == 0) return 1;

    /* bit-size estimate of ((2n-1)!!)^3 / (2n)! */
    for (i = 1; i <= 2 * n; i++) {
        int rep;
        if (i & 1) {
            for (rep = 0; rep < 3; rep++) {
                mm *= (double)i;
                while (mm >= 2.0) { mm /= 2.0; e++; }
            }
        }
        mm /= (double)i;
        while (mm < 1.0) { mm *= 2.0; e--; }
        while (mm >= 2.0) { mm /= 2.0; e++; }
    }
    bits = e + 1 + 24;
    if (bits < 1) bits = 1;
    np = (size_t)(bits / 29) + 2;

    LEN = np + 2;
    R = (uint32_t *)calloc(LEN, sizeof(uint32_t));
    Q = (uint32_t *)calloc(LEN, sizeof(uint32_t));
    Q[0] = 1;

    for (q = 0; q < np; q++) {
        uint64_t p = pr_get(q);
        uint64_t r = solve_mod(n, p);
        uint64_t rm = big_mod(R, LEN, p);
        uint64_t qm = big_mod(Q, LEN, p);
        uint64_t tt = (r + p - rm) % p * pw_mod(qm, p - 2, p) % p;
        uint64_t carry = 0;
        for (t = 0; t < LEN; t++) {
            uint64_t cur = (uint64_t)R[t] + (uint64_t)Q[t] * tt + carry;
            R[t] = (uint32_t)cur;
            carry = cur >> 32;
        }
        carry = 0;
        for (t = 0; t < LEN; t++) {
            uint64_t cur = (uint64_t)Q[t] * p + carry;
            Q[t] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }

    for (t = LEN; t > 0; t--) {
        res = res * 65536 + (seq_t)(int)(R[t - 1] >> 16);
        res = res * 65536 + (seq_t)(int)(R[t - 1] & 0xFFFFu);
    }
    free(R);
    free(Q);
    return res;
}
