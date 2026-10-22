/*
 * A006511  Largest inverse of totient function (A000010): a(n) is the largest
 *          x such that phi(x) = m, where m = A002202(n) is the n-th number in
 *          the range of phi.
 * https://oeis.org/A006511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057635(A002202(n)); computed by sieving phi(x) for x <=
 *            8*M and keeping, for each totient value m <= M, the largest x
 *            with phi(x) = m (x/phi(x) < 8 for all x in range)
 * Range:     u8 n <= 29, u16 n <= 3115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache_vals = NULL;
static uint64_t cache_len = 0;
static uint64_t cache_M = 0;

static void build(uint64_t M) {
    uint64_t L = 8 * M + 1;
    uint32_t *ph = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint64_t *best = (uint64_t *)calloc((size_t)(M + 1), sizeof(uint64_t));
    uint64_t i, j, cnt = 0;
    for (i = 0; i <= L; i++) ph[i] = (uint32_t)i;
    for (i = 2; i <= L; i++) {
        if (ph[i] == i) {
            for (j = i; j <= L; j += i) ph[j] -= ph[j] / (uint32_t)i;
        }
    }
    for (i = 1; i <= L; i++) {
        uint64_t m = ph[i];
        if (m <= M && i > best[m]) best[m] = i;
    }
    for (i = 1; i <= M; i++) if (best[i]) cnt++;
    free(cache_vals);
    cache_vals = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 1; i <= M; i++) if (best[i]) cache_vals[cnt++] = best[i];
    cache_len = cnt;
    cache_M = M;
    free(best);
    free(ph);
}

seq_t a(idx_t n) {
    if ((uint64_t)n > cache_len) {
        uint64_t M = cache_M ? cache_M : 4096;
        while (M < 8 * (uint64_t)n) M *= 2;
        build(M);
        while ((uint64_t)n > cache_len) build(cache_M * 2);
    }
    return (seq_t)cache_vals[n - 1];
}
