/*
 * A059492  Expansion of generating function A_{UO}^(1)(8n).
 * https://oeis.org/A059492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005156(n)^2, where A005156(n) = (1/2^n) *
 *            Product_{k=1..n} ((6k-2)!(2k-1)!)/((4k-1)!(4k-2)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 47 (2026-11-07)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

static long long A005156_cnt_ge(long long n, long long a, long long b, long long j) {
    long long kmin = (j + b + a - 1) / a;
    if (kmin < 1) kmin = 1;
    if (kmin > n) return 0;
    return n - kmin + 1;
}

static seq_t A005156_a(idx_t n) {
    if (n == 0) return 1;
    long long nn = (long long)n;
    long long N = 6 * nn;
    int *spf = (int *)calloc((size_t)(N + 1), sizeof(int));
    long long *ex = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (long long i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (long long j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (int)i;
        }
    }
    for (long long j = 2; j <= N; j++) {
        long long c = A005156_cnt_ge(nn, 6, 2, j) + A005156_cnt_ge(nn, 2, 1, j)
                    - A005156_cnt_ge(nn, 4, 1, j) - A005156_cnt_ge(nn, 4, 2, j);
        if (c == 0) continue;
        long long m = j;
        while (m > 1) {
            long long p = spf[m];
            m /= p;
            ex[p] += c;
        }
    }
    ex[2] -= nn;
    seq_t result = 1;
    for (long long p = 2; p <= N; p++) {
        if (spf[p] == p && ex[p] > 0)
            result *= ipow_idx((seq_t)p, (idx_t)ex[p]);
    }
    free(spf);
    free(ex);
    return result;
}

seq_t a(idx_t n) {
    seq_t v = A005156_a(n);
    return v * v;
}
