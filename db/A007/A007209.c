/*
 * A007209  Number of partitions of n into parts of sizes {a( )} is a(n).
 * https://oeis.org/A007209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f. A(x) = 1/((1-x) * Product_{k>2} (1 - x^a(k))), a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=2; a(m) = number of partitions of m into
 *            parts {1} U {a(k): 3<=k<m, a(k)<=m}, except a(m) = m when that
 *            count equals m-1 (the self-consistent part m itself)
 * Range:     u8 n <= 23, u16 n <= 74, u32 n <= 306, u64 n <= 1722,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a7209_cache = NULL;
static idx_t a7209_len = 0;

static void a7209_build(idx_t N) {
    seq_t *val = alloc_zero(N + 1);
    seq_t *dp = alloc_zero(N + 1);
    idx_t i, m, qk;
    for (i = 0; i <= N; i++) dp[i] = 1; /* part 1 */
    val[0] = 1;
    if (N >= 1) val[1] = 1;
    if (N >= 2) val[2] = 2;
    if (N >= 3) val[3] = 2;
    qk = 3;
    for (m = 4; m <= N; m++) {
        while (qk < m && val[qk] <= (seq_t)m) {
            idx_t s = (idx_t)(long long)val[qk];
            for (i = s; i <= N; i++) dp[i] += dp[i - s];
            qk++;
        }
        {
            seq_t c = dp[m];
            if (c + 1 == (seq_t)m) val[m] = (seq_t)m;
            else val[m] = c;
        }
    }
    release(dp, N + 1);
    if (a7209_cache != NULL) release(a7209_cache, a7209_len);
    a7209_cache = val;
    a7209_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a7209_len) {
        idx_t N = n + 64;
        if (N < 2 * a7209_len) N = 2 * a7209_len;
        a7209_build(N);
    }
    return a7209_cache[n];
}
