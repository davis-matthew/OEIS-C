/*
 * A007211  Oscillates under partition transform.
 * https://oeis.org/A007211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of partitions of n into parts from the set of
 *            values of b, and b(n) = number of partitions of n into parts
 *            from the set of values of a (b = A007212). Both are computed
 *            together with coin-change DP. A value equal to m can be its own
 *            part at step m, so that case is resolved by self-co...
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   add_part, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/add_part.h"

static seq_t *cache = NULL;
static idx_t cache_n = 0;

static void build(idx_t N) {
    seq_t *PA = alloc_zero(N + 1); /* partitions into parts from values of b */
    seq_t *PB = alloc_zero(N + 1); /* partitions into parts from values of a */
    seq_t *res = alloc_zero(N + 1);
    bool *inA = (bool *)calloc((size_t)N + 1, sizeof(bool));
    bool *inB = (bool *)calloc((size_t)N + 1, sizeof(bool));
    static const int ia[4] = {1, 0, 1, 0};
    static const int ib[4] = {1, 1, 0, 0};
    idx_t m;

    PA[0] = 1;
    PB[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t x = PA[m], y = PB[m];
        seq_t av = x, bv = y;
        bool found = false;
        int c;
        for (c = 0; c < 4 && !found; c++) {
            int nA = ia[c], nB = ib[c];
            bool okA, okB;
            if (nA && inA[m]) continue;
            if (nB && inB[m]) continue;
            av = x;
            if (nB) av += 1;
            bv = y;
            if (nA) bv += 1;
            okA = inA[m] ? true : ((av == (seq_t)m) == (nA != 0));
            okB = inB[m] ? true : ((bv == (seq_t)m) == (nB != 0));
            if (okA && okB) found = true;
        }
        if (!found) { av = x; bv = y; }
        res[m] = av;
        if (av >= 1 && av <= (seq_t)N) {
            idx_t v = (idx_t)(long long)av;
            if (!inA[v]) { inA[v] = true; add_part(PB, N, v); }
        }
        if (bv >= 1 && bv <= (seq_t)N) {
            idx_t v = (idx_t)(long long)bv;
            if (!inB[v]) { inB[v] = true; add_part(PA, N, v); }
        }
    }

    release(PA, N + 1);
    release(PB, N + 1);
    free(inA);
    free(inB);
    if (cache) release(cache, cache_n + 1);
    cache = res;
    cache_n = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (cache == NULL || n > cache_n) {
        idx_t N = n;
        if (cache != NULL && N < 2 * cache_n) N = 2 * cache_n;
        if (N < 64) N = 64;
        build(N);
    }
    return cache[n];
}
