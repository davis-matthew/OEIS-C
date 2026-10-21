/*
 * A212532  Number of nondecreasing sequences of n 1..4 integers with every
 *          element dividing the sequence sum.
 * https://oeis.org/A212532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6)+a(n-12)
 *            -a(n-13)-a(n-14)+a(n-16)+a(n-17)-a(n-18) Initial values up to
 *            n=19 are obtained by brute force
 * Range:     u8 n <= 21, u16 n <= 162, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

/* cache for already computed values */
static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last_computed = 0;

/* brute‑force count for n ≤ 19 */
static seq_t brute_count(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t c4 = 0; c4 <= n; ++c4) {
        for (idx_t c3 = 0; c3 <= n - c4; ++c3) {
            for (idx_t c2 = 0; c2 <= n - c4 - c3; ++c2) {
                idx_t c1 = n - c4 - c3 - c2;

                seq_t sum = (seq_t)c1
                          + (seq_t)2 * (seq_t)c2
                          + (seq_t)3 * (seq_t)c3
                          + (seq_t)4 * (seq_t)c4;

                if (c2 > 0 && sum % 2 != 0) continue;
                if (c3 > 0 && sum % 3 != 0) continue;
                if (c4 > 0 && sum % 4 != 0) continue;

                ++cnt;
            }
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* make sure the cache can hold indices up to n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = (cap > 0) ? (cap << 1) : 32;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* zero any new entries */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* compute the first 19 terms by brute force if not already done */
    if (last_computed < 19) {
        idx_t limit = 19;
        if (n < 19) limit = n;
        for (idx_t i = last_computed + 1; i <= limit; ++i) {
            cache[i] = brute_count(i);
            last_computed = i;
        }
    }

    /* recurrence for n ≥ 20 */
    if (n >= 20) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = cache[i-1] + cache[i-2];
            val -= cache[i-4];
            val -= cache[i-5];
            val += cache[i-6];
            val += cache[i-12];
            val -= cache[i-13];
            val -= cache[i-14];
            val += cache[i-16];
            val += cache[i-17];
            val -= cache[i-18];
            cache[i] = val;
            last_computed = i;
        }
    }

    return cache[n];
}
