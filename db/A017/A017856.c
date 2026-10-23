/*
 * A017856  Expansion of
 *          1/(1-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16).
 * https://oeis.org/A017856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 100, u64 n <= 191,
 *            u128 n <= 373, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=0 for 1<=n<6; for n>=6:  a(n)=∑_{k=6}^{16} a(n−k) */
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_cached = 0;          /* largest n that has been computed */

    /* Initial allocation covering 0..16 */
    if (cache == NULL) {
        const size_t init_cap = 32;        /* enough for a few initial values */
        cache = seq_alloc(init_cap);
        if (!cache) abort();
        for (size_t i = 0; i < init_cap; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;              /* a(0)=1 */
        cap = init_cap;
    }

    /* Grow cache if necessary */
    if ((idx_t)cap <= n) {
        size_t needed = (size_t)(n + 1);
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;                /* power‑of‑two growth */
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;          /* initialise new slots to zero */
        cap = new_cap;
    }

    /* Compute all values from max_cached+1 up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i < 6) {
                cache[(size_t)i] = (seq_t)0;   /* a(1) … a(5) = 0 */
            } else {
                seq_t sum = (seq_t)0;
                /* sum only over valid k with i>=k */
                for (int k = 6; k <= 16; ++k) {
                    if (i >= k)
                        sum = sum + cache[(size_t)(i - k)];
                }
                cache[(size_t)i] = sum;
            }
        }
        max_cached = n;
    }

    return cache[(size_t)n];
}
