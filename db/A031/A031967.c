/*
 * A031967  Numbers with exactly four distinct base-8 digits.
 * https://oeis.org/A031967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   returns the n-th integer whose base‑8 representation * consists
 *            of exactly four distinct digits. The implementation * performs
 *            an incremental search and caches the results
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has4Distinct, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has4Distinct.h"

seq_t a(idx_t n)
{
    /* Cache state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cap = 0;
    static seq_t last = 0;

    if (n == 0)
        return 0; /* Undefined for offset 1; returned for robustness */

    /* Grow cache until we have the n-th term */
    while (cache_len < n)
    {
        /* Find next candidate */
        seq_t candidate = last + (seq_t)1;
        while (!has4Distinct(candidate))
            candidate = candidate + (seq_t)1;

        /* Resize storage if needed */
        if (cache_len == cap)
        {
            size_t new_cap = cap ? cap * 2 : 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* Store */
        cache[cache_len] = candidate;
        cache_len++;
        last = candidate;
    }

    return cache[n - 1];
}
