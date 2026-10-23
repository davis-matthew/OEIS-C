/*
 * A165418  a(1) = 1. For n >= 2, a(n) = sum a(k), where k is over the
 *          distinct values of the substrings in binary n, and where 1 <= k <
 *          n.
 * https://oeis.org/A165418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(1) = 1, for n ≥ 2, a(n) = Σ a(k), summed over all
 *            distinct integer values k that appear as substrings of the
 *            binary representation of n, with 1 ≤ k < n
 * Range:     u8 n <= 122, u16 n <= 3967, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* ----- static cache of computed values -------------------------------- */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;          /* current allocated size */
    static uint8_t *seen     = NULL;
    static size_t seen_cap    = 0;        /* size of seen buffer */

    /* nothing defined for n==0 – return 0 */
    if (n < 1u) return 0;

    /* Ensure cache is large enough to hold indices up to n */
    size_t old_cap = cache_cap;
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* allocation failure */
        cache = tmp;
        /* zero the newly allocated part */
        memset(cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Ensure the base value a(1)=1 is stored */
    if (cache[1] == 0) {
        cache[1] = (seq_t)1;
    }

    /* If requested value already computed, return it */
    if (n < cache_cap && n > 0 && cache[n] != (seq_t)0) {
        return cache[n];
    }

    /* Ensure seen buffer can index values up to n-1 */
    if (seen_cap < (size_t)n + 1) {
        uint8_t *tmp = (uint8_t *)realloc(seen, (size_t)n + 1);
        if (!tmp) exit(1);
        seen = tmp;
        seen_cap = (size_t)n + 1;
    }

    /* Compute missing values from the smallest index that was not yet
       computed up to the requested n.  The first index that needs
       computing is max(old_cap, 2) (we skip 0 and 1). */
    size_t start = old_cap;
    if (start < 2u) start = 2u;
    for (size_t idx = start; idx <= (size_t)n; ++idx)
    {
        /* length of the binary representation of idx */
        idx_t tmp = (idx_t)idx;
        int len = 0;
        while (tmp) {
            ++len;
            tmp >>= 1;
        }

        /* Reset seen flag array for this idx */
        memset(seen, 0, (idx + 1) * sizeof(uint8_t));

        seq_t sum = 0;
        /* enumerate all substrings of the binary representation */
        for (int s = 0; s < len; ++s) {
            seq_t val = 0;
            for (int e = s; e < len; ++e) {
                idx_t bit = (idx >> (len - 1 - e)) & 1u;
                val = (val << 1) | (seq_t)bit;
                if (val == 0 || val >= (seq_t)idx)
                    continue;
                size_t k = (size_t)val;
                if (!seen[k]) {
                    seen[k] = 1;
                    sum += cache[k];   /* cache[k] is already computed */
                }
            }
        }
        cache[idx] = sum;
    }

    return cache[(size_t)n];
}
