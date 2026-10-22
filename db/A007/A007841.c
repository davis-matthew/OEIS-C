/*
 * A007841  Number of factorizations of permutations of n letters into cycles
 *          in nondecreasing length order.
 * https://oeis.org/A007841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. prod_{m>=1} 1/(1-x^m/m). Multiplying in one factor
 *            1/(1-x^m/m) at a time, with A(k) = k!*[x^k]: A(k) +=
 *            binomial(k,m)*(m-1)!*A(k-m) for k = m..n (k increasing),
 *            starting from A(0) = 1, A(k) = 0 for k > 0; then a(n) = A(n)
 *            after all m = 1..n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

static void build(idx_t N) {
    idx_t len = N + 1;
    seq_t *A = alloc_zero(len);
    A[0] = 1;
    seq_t fact = 1; /* (m-1)! */
    for (idx_t m = 1; m <= N; m++) {
        seq_t coef = fact; /* binomial(k,m)*(m-1)! = k!/(m*(k-m)!) at k = m */
        for (idx_t k = m; k <= N; k++) {
            if (k > m) {
                coef = coef * (seq_t)k / (seq_t)(k - m);
            }
            A[k] += coef * A[k - m];
        }
        fact *= (seq_t)m;
    }
    if (cache) release(cache, cache_len);
    cache = A;
    cache_len = len;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t N = n;
        if (cache_len > 0 && N < 2 * (cache_len - 1)) N = 2 * (cache_len - 1);
        if (N < 16) N = 16;
        build(N);
    }
    return cache[n];
}
