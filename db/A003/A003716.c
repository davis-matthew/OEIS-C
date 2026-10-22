/*
 * A003716  Expansion of e.g.f. tan(sinh(x)) (odd powers only).
 * https://oeis.org/A003716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k odd} T(k) * S(2n+1,k), where T(k) are the tangent
 *            numbers (tan(y) = Sum T(k) y^k/k!) and S(m,k) = m! [x^m]
 *            sinh(x)^k/k! satisfies S(m+2,k) = k^2*S(m,k) + S(m,k-2), S(1,1)
 *            = 1 (from (sinh^k/k!)'' = k^2 sinh^k/k! + sinh^(k-2)/(k-2)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   release, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cached = 0;

static void compute(idx_t N) {
    long long NN = (long long)N;
    /* tangent numbers: t[i] = T(2i-1), i = 1..N (Knuth-Buckholtz) */
    seq_t *t = zalloc_arr(N + 1);
    t[1] = (seq_t)1;
    for (long long k = 2; k <= NN; k++)
        t[k] = (seq_t)(k - 1) * t[k - 1];
    for (long long k = 2; k <= NN; k++)
        for (long long j = k; j <= NN; j++)
            t[j] = (seq_t)(j - k) * t[j - 1] + (seq_t)(j - k + 2) * t[j];

    /* s[i] = S(m, 2i+1), starting at m = 1 */
    seq_t *s = zalloc_arr(N + 1);
    s[0] = (seq_t)1;

    seq_t *res = zalloc_arr(N);
    for (long long n = 0; n < NN; n++) {
        seq_t sum = (seq_t)0;
        for (long long i = 0; i <= n; i++)
            sum += t[i + 1] * s[i];
        res[n] = sum;
        if (n + 1 < NN) {
            /* advance m -> m+2 */
            for (long long i = n + 1; i >= 0; i--) {
                seq_t k = (seq_t)(2 * i + 1);
                seq_t v = k * k * s[i];
                if (i > 0) v += s[i - 1];
                s[i] = v;
            }
        }
    }

    release(t, N + 1);
    release(s, N + 1);
    if (cache) release(cache, cached);
    cache = res;
    cached = N;
}

seq_t a(idx_t n) {
    if (n >= cached) {
        idx_t N = n + 1;
        if (N < 2 * cached) N = 2 * cached;
        if (N < 16) N = 16;
        compute(N);
    }
    return cache[n];
}
