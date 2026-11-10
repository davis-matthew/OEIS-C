/*
 * A202016  Numbers occurring only once in A063882.
 * https://oeis.org/A202016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with exactly one occurrence in A063882
 *            (A132157(k)=1); A063882: b(n)=b(n-b(n-1))+b(n-b(n-4)),
 *            b(1..4)=1; computed by generating b and counting values
 * Range:     u8 n <= 18, u16 n <= 3251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9578 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t M = 256;
    if ((size_t)n * 8 + 64 > M) M = (size_t)n * 8 + 64;
    for (;;) {
        seq_t *b = mem_new(seq_t, M + 1);
        if (b == NULL) abort();
        for (size_t i = 1; i <= M; ++i) {
            if (i <= 4) b[i] = (seq_t)1;
            else {
                size_t i1 = i - (size_t)(long long)b[i - 1];
                size_t i4 = i - (size_t)(long long)b[i - 4];
                b[i] = b[i1] + b[i4];
            }
        }
        /* threshold: min of the tail; values below it are assumed final */
        size_t V = M;
        for (size_t i = M / 2 + 1; i <= M; ++i) {
            size_t v = (size_t)(long long)b[i];
            if (v < V) V = v;
        }
        unsigned char *cnt = (unsigned char *)calloc(V + 1, 1);
        if (cnt == NULL) abort();
        for (size_t i = 1; i <= M; ++i) {
            size_t v = (size_t)(long long)b[i];
            if (v < V && cnt[v] < 2) cnt[v]++;
        }
        idx_t found = 0;
        size_t result = 0;
        for (size_t v = 1; v < V; ++v) {
            if (cnt[v] == 1) {
                if (++found == n) { result = v; break; }
            }
        }
        free(cnt);
        mem_free(b);
        if (found == n) return (seq_t)result;
        M *= 2;
    }
}
