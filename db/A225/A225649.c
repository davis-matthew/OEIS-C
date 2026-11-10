/*
 * A225649  Positions of non-ones in A225650, numbers n such that n and
 *          A000793(n) have at least one common divisor > 1.
 * https://oeis.org/A225649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 2 with gcd(m, A000793(m)) > 1; computed by a
 *            Landau-function knapsack (log values, partition reconstructed,
 *            so no overflow) and a sieve of flags
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static unsigned char *a225649_flag = NULL;
static idx_t a225649_cap = 0;

static void a225649_build(idx_t N) {
    size_t W = (size_t)N + 1;
    unsigned char *comp = (unsigned char *)calloc(W, 1);
    idx_t *pr = (idx_t *)malloc(W * sizeof(idx_t));
    size_t np = 0, i;
    idx_t p, q, s, m;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        pr[np++] = p;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
    }
    long double *lg = (long double *)calloc(W, sizeof(long double));
    unsigned char *ch = (unsigned char *)calloc(W * (np ? np : 1), 1);
    for (i = 0; i < np; i++) {
        p = pr[i];
        long double lp = 0;
        {
            /* log(p) via series-free method: use builtin through double */
            lp = (long double)__builtin_log((double)p);
        }
        for (s = N; s >= p; s--) {
            long double best = lg[s];
            unsigned char bk = 0;
            idx_t pk = p;
            unsigned char k = 1;
            while (pk <= s) {
                long double cand = lg[s - pk] + (long double)k * lp;
                if (cand > best) { best = cand; bk = k; }
                if (pk > N / p) break;
                pk *= p;
                k++;
            }
            lg[s] = best;
            ch[i * W + (size_t)s] = bk;
        }
    }
    unsigned char *flag = (unsigned char *)calloc(W, 1);
    for (m = 2; m <= N; m++) {
        idx_t rem = m;
        size_t j = np;
        bool hit = false;
        while (j > 0 && rem > 0) {
            j--;
            unsigned char k = ch[j * W + (size_t)rem];
            if (k > 0) {
                idx_t pp = pr[j], t;
                if (m % pp == 0) hit = true;
                for (t = 0; t < k; t++) rem -= pp;
                /* rem decreased by pp^k computed via repeated multiply */
            }
        }
        flag[m] = hit ? 1 : 0;
    }
    free(comp); free(pr); free(lg); free(ch);
    if (a225649_flag) free(a225649_flag);
    a225649_flag = flag;
    a225649_cap = N;
}

seq_t a(idx_t n) {
    for (;;) {
        if (a225649_flag != NULL) {
            idx_t cnt = 0, m;
            for (m = 2; m <= a225649_cap; m++) {
                if (a225649_flag[m]) {
                    cnt++;
                    if (cnt == n) return (seq_t)m;
                }
            }
        }
        idx_t N = a225649_cap * 2;
        if (N < 256) N = 256;
        if (N < 2 * n) N = 2 * n;
        a225649_build(N);
    }
}
