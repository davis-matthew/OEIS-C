/*
 * A007325  G.f.: Product_{k>0}
 *          (1-x^(5k-1))*(1-x^(5k-4))/((1-x^(5k-2))*(1-x^(5k-3))).
 * https://oeis.org/A007325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. A(x) = N(x)/D(x) with N(x) = Sum_{k in Z} (-1)^k
 *            x^(k*(5k+3)/2), D(x) = Sum_{k in Z} (-1)^k x^(k*(5k+1)/2); since
 *            D(0)=1, a(m) = [x^m]N - Sum_{j>=1} [x^j]D * a(m-j)
 * Range:     i8 n <= 64, i16 n <= 184, i32 n <= 585, i64 n <= 2000,
 *            i128 n <= 7235, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    long long N = (long long)n;
    int *nc = (int *)calloc((size_t)N + 1, sizeof(int));
    long long *de = (long long *)malloc(sizeof(long long) * 64);
    int *ds = (int *)malloc(sizeof(int) * 64);
    long long dcnt = 0, dcap = 64;
    long long k;

    /* numerator coefficients (dense, small ints) */
    for (k = 0;; k++) {
        long long e1 = k * (5 * k + 3) / 2;   /* +k */
        long long e2 = k * (5 * k - 3) / 2;   /* -k */
        int s = (k % 2 == 0) ? 1 : -1;
        if (e1 > N && e2 > N) break;
        if (e1 <= N) nc[e1] += s;
        if (k > 0 && e2 <= N) nc[e2] += s;
    }
    /* denominator terms with exponent >= 1 (sparse) */
    for (k = 1;; k++) {
        long long e1 = k * (5 * k + 1) / 2;
        long long e2 = k * (5 * k - 1) / 2;
        int s = (k % 2 == 0) ? 1 : -1;
        if (e2 > N) break;
        if (dcnt + 2 > dcap) {
            dcap *= 2;
            de = (long long *)realloc(de, sizeof(long long) * (size_t)dcap);
            ds = (int *)realloc(ds, sizeof(int) * (size_t)dcap);
        }
        de[dcnt] = e2; ds[dcnt] = s; dcnt++;
        if (e1 <= N) { de[dcnt] = e1; ds[dcnt] = s; dcnt++; }
    }

    seq_t *c = alloc_zero(n + 1);
    for (long long m = 0; m <= N; m++) {
        seq_t v = 0;
        v += nc[m];
        for (long long j = 0; j < dcnt; j++) {
            long long e = de[j];
            if (e > m) continue;
            if (ds[j] > 0) v -= c[m - e];
            else v += c[m - e];
        }
        c[m] = v;
    }
    seq_t result = c[N];
    release(c, n + 1);
    free(nc);
    free(de);
    free(ds);
    return result;
}
