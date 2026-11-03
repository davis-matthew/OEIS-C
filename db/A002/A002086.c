/*
 * A002086  Number of circulant tournaments on 2n+1 nodes up to Cayley
 *          isomorphism.
 * https://oeis.org/A002086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/phi(N)) * Sum_{u in (Z/NZ)^*} fix(u), N = 2n+1
 *            (Burnside). fix(u) = 0 if -1 lies in <u mod d> for some divisor
 *            d>1 of N, else 2^(Sum_{d|N,d>1} phi(d)/(2*ord_d(u)))
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 184 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

static int a2086_factor(uint64_t x, uint64_t *pr) {
    int k = 0;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            pr[k++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) pr[k++] = x;
    return k;
}

static uint64_t a2086_pm(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t N = 2 * (uint64_t)n + 1;
    uint64_t np[64];
    int nnp = a2086_factor(N, np);

    /* divisors of N */
    size_t cap = 1;
    {
        uint64_t x = N;
        for (int i = 0; i < nnp; i++) {
            int ex = 0;
            while (x % np[i] == 0) { x /= np[i]; ex++; }
            cap *= (size_t)(ex + 1);
        }
    }
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t nd = 0;
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            divs[nd++] = d;
            if (d != N / d) divs[nd++] = N / d;
        }
    }

    uint64_t *dphi = (uint64_t *)malloc(nd * sizeof(uint64_t));
    uint64_t (*dpr)[64] = (uint64_t (*)[64])malloc(nd * sizeof(*dpr));
    int *dnp = (int *)malloc(nd * sizeof(int));
    for (size_t j = 0; j < nd; j++) {
        uint64_t d = divs[j], ph = d;
        for (int i = 0; i < nnp; i++)
            if (d % np[i] == 0) ph = ph / np[i] * (np[i] - 1);
        dphi[j] = ph;
        dnp[j] = a2086_factor(ph, dpr[j]);
    }

    uint64_t *cnt = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    uint64_t phiN = 0;

    for (uint64_t u = 1; u < N; u++) {
        if (gcd_u64(u, N) != 1) continue;
        phiN++;
        uint64_t e = 0;
        bool ok = true;
        for (size_t j = 0; j < nd && ok; j++) {
            uint64_t d = divs[j];
            if (d == 1) continue;
            uint64_t ph = dphi[j], k = ph;
            for (int i = 0; i < dnp[j]; i++) {
                uint64_t p = dpr[j][i];
                while (k % p == 0 && a2086_pm(u, k / p, d) == 1) k /= p;
            }
            if (k % 2 == 0 && a2086_pm(u, k / 2, d) == d - 1) {
                ok = false;
                break;
            }
            e += ph / (2 * k);
        }
        if (ok) cnt[e]++;
    }

    /* Sum cnt[e] * 2^e / phiN, tracking 2^e = q*phiN + r */
    seq_t Q = 0, q = 0;
    uint64_t R = 0, r = 1;
    for (uint64_t e = 0; e <= (uint64_t)n; e++) {
        if (cnt[e]) {
            Q += q * (seq_t)cnt[e];
            R += cnt[e] * r;
            R %= phiN * phiN;
        }
        q = q + q;
        r = 2 * r;
        if (r >= phiN) { r -= phiN; q += 1; }
    }
    /* R is a multiple of phiN in total; reduce: R holds sum mod phiN^2 */
    Q += (seq_t)(R / phiN);

    free(divs); free(dphi); free(dpr); free(dnp); free(cnt);
    return Q;
}
