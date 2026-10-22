/*
 * A001486  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^8 in powers of x.
 * https://oeis.org/A001486
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] (Q(x))^8, where Q(x) = Product_{j>=1} (1-(-x)^j) -
 *            1 = Sum_{k!=0} (-1)^k (-1)^(k(3k-1)/2) x^(k(3k-1)/2) (Euler's
 *            pentagonal number theorem). Q is sparse with coefficients +-1,
 *            so Q^8 is built by 7 sparse multiplications
 * Range:     i8 n <= 13, i16 n <= 176, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a001486_cache = NULL;
static idx_t a001486_cap = 0;

static void a001486_build(idx_t N) {
    idx_t *ex = (idx_t *)malloc((N + 2) * sizeof(idx_t));
    int *sg = (int *)malloc((N + 2) * sizeof(int));
    idx_t nt = 0;
    for (idx_t k = 1;; k++) {
        idx_t m1 = k * (3 * k - 1) / 2;
        idx_t m2 = k * (3 * k + 1) / 2;
        if (m1 > N) break;
        int sk = (k % 2 == 0) ? 1 : -1;
        ex[nt] = m1; sg[nt] = sk * ((m1 % 2 == 0) ? 1 : -1); nt++;
        if (m2 <= N) { ex[nt] = m2; sg[nt] = sk * ((m2 % 2 == 0) ? 1 : -1); nt++; }
    }
    /* sort exponents ascending (insertion sort, few terms) */
    for (idx_t i = 1; i < nt; i++) {
        idx_t e = ex[i]; int s = sg[i]; idx_t j = i;
        while (j > 0 && ex[j - 1] > e) { ex[j] = ex[j - 1]; sg[j] = sg[j - 1]; j--; }
        ex[j] = e; sg[j] = s;
    }

    seq_t *cur = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *nxt = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    memset(cur, 0, (N + 1) * sizeof(seq_t));
    memset(nxt, 0, (N + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= N; i++) cur[i] = 0;
    for (idx_t t = 0; t < nt; t++) {
        if (sg[t] > 0) cur[ex[t]] = 1; else cur[ex[t]] = -1;
    }

    for (idx_t p = 2; p <= 8; p++) {
        for (idx_t i = 0; i <= N; i++) {
            nxt[i] = 0;
            if (i < p) continue;
            for (idx_t t = 0; t < nt && ex[t] <= i - (p - 1); t++) {
                if (sg[t] > 0) nxt[i] += cur[i - ex[t]];
                else nxt[i] -= cur[i - ex[t]];
            }
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    free(nxt);
    free(ex);
    free(sg);
    if (a001486_cache) free(a001486_cache);
    a001486_cache = cur;
    a001486_cap = N;
}

seq_t a(idx_t n) {
    if (n < 8) return 0;
    if (a001486_cache == NULL || n > a001486_cap) {
        idx_t N = a001486_cap * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a001486_build(N);
    }
    return a001486_cache[n];
}
