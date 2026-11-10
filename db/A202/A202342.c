/*
 * A202342  Numbers occurring exactly twice in Hofstadter H-sequence A005374.
 * https://oeis.org/A202342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th value occurring exactly twice in A005374, where
 *            A005374(k) = k - A005374(A005374(A005374(k-1))), A005374(0) = 0;
 *            computed by generating A005374 and scanning runs of length 2
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9265 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static long long *tw_terms = NULL;
static idx_t tw_cnt = 0;

seq_t a(idx_t n)
{
    if (n >= 1 && n <= tw_cnt) return (seq_t)tw_terms[n - 1];
    idx_t N = 4 * n + 64;
    for (;;) {
        long long *h = (long long *)malloc(sizeof(long long) * (size_t)(N + 1));
        if (h == NULL) return (seq_t)0;
        h[0] = 0;
        for (idx_t i = 1; i <= N; i++) {
            long long x = h[i - 1];
            x = h[x];
            x = h[x];
            h[i] = (long long)i - x;
        }
        long long *terms = (long long *)malloc(sizeof(long long) * (size_t)(N + 1));
        if (terms == NULL) { free(h); return (seq_t)0; }
        idx_t cnt = 0;
        idx_t s = 1;
        for (idx_t i = 2; i <= N; i++) {
            if (h[i] != h[i - 1]) {
                if (i - s == 2) terms[cnt++] = h[i - 1];
                s = i;
            }
        }
        free(h);
        if (cnt >= n) {
            if (tw_terms != NULL) free(tw_terms);
            tw_terms = terms;
            tw_cnt = cnt;
            return (seq_t)tw_terms[n - 1];
        }
        free(terms);
        N *= 2;
    }
}
