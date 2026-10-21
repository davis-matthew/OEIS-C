/*
 * A226761  G.f.: 1 / (1 + 12*x*G(x)^2 - 13*x*G(x)^3) where G(x) = 1 +
 *          x*G(x)^4 is the g.f. of A002293.
 * https://oeis.org/A226761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(3*k, n-k) * C(4*n-3*k, k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;       /* sequence cache */
static size_t cache_len = 0;     /* number of cached values */

/* Expand the cache up to index n and compute missing terms */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1);               /* fatal error if memory cannot be allocated */
    cache = tmp;

    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) {            /* a(0) = 1 */
            cache[i] = 1;
            continue;
        }

        idx_t N = (idx_t)i;
        idx_t min_k = (N + 3) / 4;     /* ceil(N/4) – first k with a non‑zero term */

        /* initial values */
        seq_t b = binom(3 * min_k, N - min_k);  /* C(3k, N-k) at k=min_k   */
        seq_t c = 1;                            /* C(4N, 0)                 */
        seq_t sum = 0;

        /* move c from k=0 to k=min_k                                    */
        for (idx_t k = 0; k < min_k; ++k) {
            long long Ncurr = 4LL * N - 3LL * k;
            long long num_c = (Ncurr - k) * (Ncurr - k - 1) *
                              (Ncurr - k - 2) * (Ncurr - k - 3);
            long long den_c = (k + 1) * Ncurr * (Ncurr - 1) *
                              (Ncurr - 2);
            c = c * (seq_t)num_c / (seq_t)den_c;
        }

        /* first term (k = min_k) */
        sum = b * c;

        /* iterate for k = min_k .. N-1 */
        for (idx_t k = min_k; k < N; ++k) {
            /* update b to b_{k+1} */
            long long num_b = (3LL * k + 1) * (3LL * k + 2) *
                              (3LL * k + 3) * ((long long)N - (long long)k);
            long long den_b = (4LL * k + 4 - (long long)N) *
                              (4LL * k + 3 - (long long)N) *
                              (4LL * k + 2 - (long long)N) *
                              (4LL * k + 1 - (long long)N);
            b = b * (seq_t)num_b / (seq_t)den_b;

            /* update c to c_{k+1} */
            long long Ncurr = 4LL * N - 3LL * k;
            long long num_c = (Ncurr - k) * (Ncurr - k - 1) *
                              (Ncurr - k - 2) * (Ncurr - k - 3);
            long long den_c = (k + 1) * Ncurr * (Ncurr - 1) *
                              (Ncurr - 2);
            c = c * (seq_t)num_c / (seq_t)den_c;

            /* accumulate term */
            sum += b * c;
        }

        cache[i] = sum;
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
