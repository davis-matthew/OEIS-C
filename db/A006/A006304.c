/*
 * A006304  Coefficients of the '2nd-order' mock theta function A(q).
 * https://oeis.org/A006304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: Sum_{m>=0} q^((m+1)^2) * Product_{k=1..m} (1+q^(2k-1)) /
 *            Product_{k=0..m} (1-q^(2k+1))^2
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 145, u64 n <= 497,
 *            u128 n <= 1811, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cache_n = 0; /* cache valid for indices 0..cache_n when cache != NULL */

static void compute(idx_t N) {
    seq_t *c = alloc_zero(N + 1);
    seq_t *t = alloc_zero(N + 1);
    t[0] = 1;
    for (idx_t m = 0; (m + 1) * (m + 1) <= N; m++) {
        idx_t s = (m + 1) * (m + 1);
        idx_t L = N - s;
        if (m >= 1) {
            idx_t e = 2 * m - 1;
            for (idx_t i = L; i >= e; i--) t[i] += t[i - e];
        }
        {
            idx_t e = 2 * m + 1;
            for (int r = 0; r < 2; r++)
                for (idx_t i = e; i <= L; i++) t[i] += t[i - e];
        }
        for (idx_t i = 0; i <= L; i++) c[i + s] += t[i];
    }
    release(t, N + 1);
    if (cache) release(cache, cache_n + 1);
    cache = c;
    cache_n = N;
}

seq_t a(idx_t n) {
    if (!cache || n > cache_n) {
        idx_t N = n;
        if (cache && N < 2 * cache_n) N = 2 * cache_n;
        if (N < 64) N = 64;
        compute(N);
    }
    return cache[n];
}
