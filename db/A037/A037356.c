/*
 * A037356  Numbers whose base-4 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑4 and base‑9 expansions have no
 *            digits in common
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mask_base4, mask_base9_upto3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mask_base4.h"
#include "../utils/mask_base9_upto3.h"

/*  True if x has no common digit 0‑3 in its base‑4 and base‑9 representations. */
static inline bool has_no_common_digits(seq_t v)
{
    return (mask_base4(v) & mask_base9_upto3(v)) == 0;
}

/*  Return the nth member of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t      *cache = NULL;  /* cached sequence values              */
    static size_t     cap   = 0;     /* allocated capacity                   */
    static size_t     size  = 0;     /* number of values in the cache       */
    static seq_t      next_candidate = 1; /* next number to test */

    if (n == 0)            /* the offset is 1, so a(0) is unused */
        return 0;

    if (size >= (size_t)n) /* we already have the nth value */
        return cache[n - 1];

    /* grow the cache if necessary */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n)
            newcap = n;
        /* cast required for C++ compilers; harmless for C */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* search for the remaining values until we fill up to n */
    while (size < (size_t)n) {
        while (!has_no_common_digits(next_candidate))
            ++next_candidate;
        cache[size++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
