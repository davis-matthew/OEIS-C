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
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool has4Distinct(seq_t x)
{
    uint16_t mask = 0;
    while (x != 0)
    {
        seq_t rem = x % (seq_t)8;
        mask |= 1u << (unsigned)rem;
        x = x / (seq_t)8;
    }
    /* Count set bits in mask */
    unsigned cnt = 0;
    while (mask)
    {
        cnt += mask & 1;
        mask >>= 1;
    }
    return cnt == 4;
}

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
