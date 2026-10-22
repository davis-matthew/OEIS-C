/*
 * A007896  Psi_c(n), where Product_{k>1} 1/(1-1/k^s)^phi(k) = Sum_{k>0}
 *          psi_c(k)/k^s.
 * https://oeis.org/A007896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dirichlet series product: a(n) = coefficient of n^-s in
 *            Product_{k>1} 1/(1-k^-s)^phi(k); computed by applying each
 *            factor in place (descending m) with coefficients
 *            binomial(j+phi(k)-1, j)
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t cache_len = 0; /* entries valid for indices 1..cache_len-1 */

static void build(idx_t N) {
    idx_t i, p, k, m;
    idx_t *phi = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    seq_t *f;
    for (i = 0; i <= N; i++) phi[i] = i;
    for (p = 2; p <= N; p++) {
        if (phi[p] == p) {
            for (i = p; i <= N; i += p) phi[i] -= phi[i] / p;
        }
    }
    f = alloc_zero(N + 1);
    f[1] = 1;
    for (k = 2; k <= N; k++) {
        idx_t ph = phi[k];
        idx_t top = N / k;
        for (m = top; m >= 1; m--) {
            seq_t base, c;
            idx_t idx, j;
            if (f[m] == 0) continue;
            base = f[m];
            c = 1;
            idx = m;
            j = 0;
            while (idx <= N / k) {
                idx *= k;
                j++;
                c = c * (seq_t)(ph + j - 1) / (seq_t)j;
                f[idx] += base * c;
            }
        }
    }
    free(phi);
    if (cache) release(cache, cache_len);
    cache = f;
    cache_len = N + 1;
}

seq_t a(idx_t n) {
    if (n + 1 > cache_len) {
        idx_t N = n;
        if (N < 2 * (cache_len ? cache_len - 1 : 0)) N = 2 * (cache_len - 1);
        if (N < 64) N = 64;
        build(N);
    }
    return cache[n];
}
