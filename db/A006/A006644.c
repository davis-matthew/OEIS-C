/*
 * A006644  Indices of records in Landau's function A000793.
 * https://oeis.org/A006644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A000793(k) > A000793(k-1) (k=0 counts);
 *            search: knapsack DP over prime powers for log g(k) = max sum of
 *            log(p^e) with sum p^e <= k, records are where it strictly
 *            increases
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static idx_t *rec = NULL;
static idx_t rec_count = 0;

static void compute_records(idx_t N) {
    long double *f = (long double *)malloc((size_t)(N + 1) * sizeof(long double));
    bool *comp = (bool *)calloc((size_t)(N + 1), sizeof(bool));
    idx_t s, i;
    uint64_t p;
    for (s = 0; s <= N; s++) f[s] = 0.0L;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= N; i += p) comp[i] = true;
        long double lq[64];
        uint64_t qv[64];
        int nk = 0;
        uint64_t q;
        for (q = p; q <= N; q *= p) {
            qv[nk] = q;
            lq[nk] = logl((long double)q);
            nk++;
        }
        for (s = N; s >= p; s--) {
            long double best = f[s];
            int k;
            for (k = 0; k < nk && qv[k] <= s; k++) {
                long double v = f[s - qv[k]] + lq[k];
                if (v > best) best = v;
            }
            f[s] = best;
        }
    }
    free(rec);
    rec = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    rec_count = 0;
    rec[rec_count++] = 0;
    for (s = 1; s <= N; s++)
        if (f[s] > f[s - 1]) rec[rec_count++] = s;
    free(f);
    free(comp);
}

seq_t a(idx_t n) {
    static idx_t limit = 0;
    if (rec_count < n) {
        idx_t N = limit ? limit : 64;
        while (N < 2 * n) N *= 2;
        for (;;) {
            compute_records(N);
            limit = N;
            if (rec_count >= n) break;
            N *= 2;
        }
    }
    return (seq_t)rec[n - 1];
}
