/*
 * A217602  Let a(1) = 1. If n and a(n-1) have the same parity, a(n) = n +
 *          a(n-1), otherwise a(n) = abs(n - a(n-1)).
 * https://oeis.org/A217602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)= n+a(n-1) if parity(n)=parity(a(n-1)),
 *            else a(n)=|n-a(n-1)|
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

/* External linkage function computing OEIS A217602 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] unused */
    static size_t cap = 0;
    static idx_t computed = 0;        /* highest index already computed */

    if (n == 0) return 0;            /* safety, offset starts at 1 */

    if (n > computed) {
        /* Ensure the cache can hold indices up to n */
        ensure_cache(n, &cache, &cap);

        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];
                seq_t curr;
                /* check parity of i and prev */
                if ((i & 1ULL) == (prev & 1ULL)) {
                    curr = prev + (seq_t)i;
                } else {
                    curr = (prev >= (seq_t)i) ? prev - (seq_t)i
                                              : (seq_t)i - prev;
                }
                cache[i] = curr;
            }
        }
        computed = n;
    }

    return cache[n];
}
