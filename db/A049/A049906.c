/*
 * A049906  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A049906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} a(k) - a(n-1-2^p) with p =
 *            floor_log2(n-2) Initial values: a(1)=1, a(2)=2, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static seq_t *prefix = NULL;
    static size_t max_n = 0;
    static size_t cap = 0;

    /* Ensure capacity for indices up to n (1‑based). */
    if (n > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n) newcap = (size_t)n;
        cache   = (seq_t*)realloc(cache,   sizeof(seq_t)*(newcap+1));
        prefix  = (seq_t*)realloc(prefix,  sizeof(seq_t)*(newcap+1));
        if (cap == 0) {          /* first allocation */
            prefix[0] = 0;       /* Cumulative sum for 0 terms */
        }
        cap = newcap;
    }

    if (n <= max_n)
        return cache[n];

    /* Compute all terms from max_n+1 up to n. */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[i] = 1;
            prefix[i] = cache[i];
        } else if (i == 2 || i == 3) {
            cache[i] = 2;
            prefix[i] = prefix[i-1] + cache[i];
        } else {
            /* p = floor_log2(i-2) */
            idx_t p = floor_log2_uint64(i - 2u);
            size_t power = (size_t)1 << p;          /* 2^p */
            idx_t m = (idx_t)(i - 1) - power;       /* n-1-2^p */
            cache[i] = prefix[i-1] - cache[m];
            prefix[i] = prefix[i-1] + cache[i];
        }
    }

    max_n = n;
    return cache[n];
}
