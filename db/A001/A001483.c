/*
 * A001483  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^5 in powers of x.
 * https://oeis.org/A001483
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] (Product_{j>=1} (1-(-x)^j) - 1)^5, using Euler's
 *            pentagonal theorem: Product_{j>=1} (1-x^j) = 1 + Sum_{k>=1}
 *            (-1)^k (x^(k(3k-1)/2) + x^(k(3k+1)/2))
 * Range:     i8 n <= 27, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t N = n + 1;
    idx_t cap = 16, cnt = 0, k, i, t, j;
    idx_t *ex = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    int *sg = (int *)malloc((size_t)cap * sizeof(int));
    long long *cur, *nxt, *tmp;
    long long result = 0;
    seq_t r;

    /* sparse terms of P(x) = Q(-x) - 1 */
    for (k = 1;; k++) {
        idx_t e1 = k * (3 * k - 1) / 2;
        idx_t e2 = k * (3 * k + 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (e1 > n) break;
        if (cnt + 2 > cap) {
            cap *= 2;
            ex = (idx_t *)realloc(ex, (size_t)cap * sizeof(idx_t));
            sg = (int *)realloc(sg, (size_t)cap * sizeof(int));
        }
        ex[cnt] = e1; sg[cnt] = (e1 & 1) ? -s : s; cnt++;
        if (e2 <= n) { ex[cnt] = e2; sg[cnt] = (e2 & 1) ? -s : s; cnt++; }
    }

    cur = (long long *)calloc((size_t)N, sizeof(long long));
    nxt = (long long *)calloc((size_t)N, sizeof(long long));
    for (j = 0; j < cnt; j++) cur[ex[j]] += sg[j];

    /* cur = P^4 */
    for (t = 2; t <= 4; t++) {
        for (i = 0; i < N; i++) nxt[i] = 0;
        for (i = 0; i < N; i++) {
            long long c = cur[i];
            if (c == 0) continue;
            for (j = 0; j < cnt; j++) {
                idx_t m = i + ex[j];
                if (m > n) break;
                nxt[m] += sg[j] * c;
            }
        }
        tmp = cur; cur = nxt; nxt = tmp;
    }

    /* coefficient of x^n in P * P^4 */
    for (j = 0; j < cnt; j++) {
        if (ex[j] > n) break;
        result += sg[j] * cur[n - ex[j]];
    }

    free(cur);
    free(nxt);
    free(ex);
    free(sg);
    r = (seq_t)result;
    return r;
}
