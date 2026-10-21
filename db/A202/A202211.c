/*
 * A202211  a(1)=2, a(2)=3, for n >= 3, a(n) = 2*(gpf(a(n-1)) + gpf(a(n-2))) +
 *          1, where gpf(n) is the greatest prime factor of n.
 * https://oeis.org/A202211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3, for n >= 3, a(n) = 2*(gpf(a(n-1)) + gpf(a(n-2)))
 *            + 1
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      // undefined for n < 1, harmless

    static seq_t *cache = NULL;
    static bool *seen  = NULL;
    static size_t cap = 0;

    /* grow cache and seen arrays if necessary */
    if ((size_t)n > cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        bool *s_tmp = (bool*)realloc(seen, newcap * sizeof(bool));
        if (!s_tmp) abort();
        seen = s_tmp;
        for (size_t i = cap; i < newcap; ++i) seen[i] = false;
        cap = newcap;
    }

    /* compute all values up to n, memoising results */
    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);
        if (seen[idx]) continue;          // already computed

        if (i == 1) {
            cache[idx] = 2;
        } else if (i == 2) {
            cache[idx] = 3;
        } else {
            seq_t prev1 = cache[(size_t)(i - 2)];   /* a(i-1) */
            seq_t prev2 = cache[(size_t)(i - 3)];   /* a(i-2) */
            uint64_t g1 = gpf((uint64_t)prev1);
            uint64_t g2 = gpf((uint64_t)prev2);
            cache[idx] = 2 * ( (seq_t)g1 + (seq_t)g2 ) + 1;
        }
        seen[idx] = true;
    }

    return cache[(size_t)(n - 1)];
}
