/*
 * A006169  Number of factorization patterns of polynomials of degree n over
 *          F_4.
 * https://oeis.org/A006169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of b(n) = sum_{d|n, A027377(d)>=n/d} 1,
 *            i.e. G.f. Product_{d>=1} Product_{j=1..A027377(d)}
 *            1/(1-x^(d*j)). Arbitrary precision: a(0)=1, a(n) =
 *            (1/n)*Sum_{k=1..n} c(k)*a(n-k), c(k)=Sum_{d|k} d*b(d). Fixed
 *            width: the product is expanded by repeated additions (exact
 *            modulo 2^w)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 71, u64 n <= 214,
 *            u128 n <= 681, big any n
 * Helpers:   alloc_zero, mobius_u, nirr, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/mobius_u.h"
#include "../utils/nirr.h"

static seq_t *cache_a = NULL;
static uint64_t cache_len = 0;

static void build(uint64_t M) {
    uint64_t *b = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
    uint64_t d, k, i;
    seq_t *arr;
    for (d = 1; d <= M; d++) {
        uint64_t nd = nirr(d);
        for (k = 1; d * k <= M; k++)
            if (nd >= k) b[d * k]++;
    }
    arr = alloc_zero(M + 1);
    arr[0] = 1;
    if (seq_is_fixed_width()) {
        uint64_t m, t;
        for (m = 1; m <= M; m++)
            for (t = 0; t < b[m]; t++)
                for (i = m; i <= M; i++)
                    arr[i] += arr[i - m];
    } else {
        uint64_t *c = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
        for (d = 1; d <= M; d++)
            if (b[d])
                for (k = d; k <= M; k += d) c[k] += d * b[d];
        for (i = 1; i <= M; i++) {
            seq_t s = 0;
            for (k = 1; k <= i; k++)
                if (c[k]) s += arr[i - k] * c[k];
            arr[i] = s / (seq_t)i;
        }
        free(c);
    }
    free(b);
    if (cache_a) release(cache_a, cache_len);
    cache_a = arr;
    cache_len = M + 1;
}

seq_t a(idx_t n) {
    if ((uint64_t)n + 1 > cache_len) {
        uint64_t M = (uint64_t)n;
        if (cache_len > 0 && 2 * (cache_len - 1) > M) M = 2 * (cache_len - 1);
        if (M < 16) M = 16;
        build(M);
    }
    return cache_a[n];
}
