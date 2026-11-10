/*
 * A227096  Self-convolution of A013999.
 * https://oeis.org/A227096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A013999(k)*A013999(n-k), where A013999(0)=1,
 *            A013999(1)=1, A013999(2)=2, A013999(n) = (n+3)*A013999(n-1) -
 *            (2n+1)*A013999(n-2) + n*A013999(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 179 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A013999_a(idx_t n)
{
    static seq_t *cache        = NULL;
    static idx_t cache_cap    = 0;
    static idx_t cache_filled = 0;

    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
        if (!tmp) exit(1);
        cache     = tmp;
        cache_cap = new_cap;
    }

    if (cache_filled == 0) {
        cache[0] = 1;
        cache_filled = 1;
    }
    if (cache_filled == 1 && n >= 1) {
        cache[1] = 1;
        cache_filled = 2;
    }
    if (cache_filled == 2 && n >= 2) {
        cache[2] = 2;
        cache_filled = 3;
    }

    if (cache_filled <= n) {
        for (idx_t i = cache_filled; i <= n; ++i) {
            seq_t t1 = (seq_t)(i + 3)   * cache[i - 1];
            seq_t t2 = (seq_t)(2 * i + 1) * cache[i - 2];
            seq_t t3 = (seq_t)(i)       * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_filled = n + 1;
    }

    return cache[n];
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    A013999_a(n); /* ensure terms up to n are computed */
    for (idx_t k = 0; k <= n; ++k) {
        sum += A013999_a(k) * A013999_a(n - k);
    }
    return sum;
}
