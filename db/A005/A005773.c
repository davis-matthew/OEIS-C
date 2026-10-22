/*
 * A005773  Number of directed animals of size n (or directed n-ominoes in
 *          standard position).
 * https://oeis.org/A005773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   n*a(n) = 2*n*a(n-1) + 3*(n-2)*a(n-2), a(0) = a(1) = 1
 *            (arbitrary-precision build); fixed-width builds count Motzkin
 *            path prefixes of length n-1 by height DP (additions only, exact
 *            mod 2^w)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t a_dp(idx_t n) {
    idx_t L = n - 1, h, s;
    seq_t *cur = alloc_zero(L + 2);
    seq_t *nxt = alloc_zero(L + 2);
    seq_t total = 0;
    cur[0] = 1;
    for (s = 0; s < L; s++) {
        for (h = 0; h <= s + 1 && h <= L; h++) {
            seq_t v = cur[h] + cur[h + 1];
            if (h > 0) v += cur[h - 1];
            nxt[h] = v;
        }
        { seq_t *t = cur; cur = nxt; nxt = t; }
    }
    for (h = 0; h <= L; h++) total += cur[h];
    release(cur, L + 2);
    release(nxt, L + 2);
    return total;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    idx_t i, old_len;

    if (n == 0 || n == 1) return 1;
    if (!seq_is_big()) return a_dp(n);

    if (cache == NULL) {
        cache_len = 2;
        cache = alloc_zero(cache_len);
        cache[0] = 1;
        cache[1] = 1;
    }
    if (n < cache_len) return cache[n];

    old_len = cache_len;
    cache = grow_zero(cache, old_len, n + 1);
    cache_len = n + 1;
    for (i = old_len; i <= n; i++) {
        seq_t num = (seq_t)2 * (seq_t)i * cache[i - 1]
                  + (seq_t)3 * (seq_t)(i - 2) * cache[i - 2];
        cache[i] = num / (seq_t)i;
    }
    return cache[n];
}
