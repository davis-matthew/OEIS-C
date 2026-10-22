/*
 * A002049  Prime numbers of measurement.
 * https://oeis.org/A002049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..n} A002048(k), where A002048 is built by a
 *            sieve: each next term is the least number above the previous one
 *            that is not a sum of two or more consecutive earlier terms
 * Range:     u8 n <= 15, u16 n <= 219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a002049_T = 0;
static idx_t a002049_cnt = 0;

static void a002049_build(idx_t N) {
    uint64_t L = (uint64_t)N * 16 + 64;
    for (;;) {
        unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
        uint64_t *t = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
        idx_t k = 0;
        uint64_t c = 1;
        int ok = 1;
        while (k < N) {
            if (c > L) { ok = 0; break; }
            if (!mark[c]) {
                long long j;
                uint64_t s = c;
                t[k++] = c;
                for (j = (long long)k - 2; j >= 0; j--) {
                    s += t[j];
                    if (s > L) break;
                    mark[s] = 1;
                }
            }
            c++;
        }
        free(mark);
        if (ok) {
            free(a002049_T);
            a002049_T = t;
            a002049_cnt = N;
            return;
        }
        free(t);
        L *= 2;
    }
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i;
    if (n > a002049_cnt) {
        idx_t N = n;
        if (N < 2 * a002049_cnt) N = 2 * a002049_cnt;
        if (N < 64) N = 64;
        a002049_build(N);
    }
    for (i = 0; i < n; i++)
        sum += (seq_t)a002049_T[i];
    return sum;
}
