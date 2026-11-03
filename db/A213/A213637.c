/*
 * A213637  Values of n for which A213636(n) = 1.
 * https://oeis.org/A213637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the index i of the n‑th occurrence of i mod LND(i) == 1 *
 *            where LND(i) is the least integer ≥ 2 that does not divide i.
 *            This * implements OEIS sequence A213637
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 861 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

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

/* Static cache for memoization between calls. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;  /* number of elements stored */
static size_t seq_cap = 0;  /* allocated capacity of seq_cache */
static idx_t processed = 0;/* last integer i that has been examined */

/* Compute the least non‑divisor of i (i ≥ 1). */
static idx_t least_nondivisor(idx_t i)
{
    for (idx_t k = 2; ; ++k)
        if (i % k != 0)
            return k;
}

/* Ensure seq_cache has at least 'need' slots available. */
static void ensure_cache_cap(size_t need)
{
    if (seq_cap < need) {
        size_t newcap = seq_cap ? seq_cap * 2 : 128;
        while (newcap < need)
            newcap *= 2;
        seq_t *newptr = seq_realloc(seq_cache, seq_cap, newcap);
        if (!newptr) abort();          /* memory exhaustion */
        seq_cache = newptr;
        seq_cap   = newcap;
    }
}

/* Append a value to the cache. */
static void append_value(seq_t v)
{
    ensure_cache_cap(seq_len + 1);
    seq_cache[seq_len++] = v;
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, avoid UB */

    /* Generate terms until the cache holds at least n elements. */
    while (seq_len < (size_t)n) {
        ++processed;                     /* next integer to examine */
        idx_t lnd = least_nondivisor(processed);
        if (processed % lnd == 1)
            append_value((seq_t)processed);
    }
    return seq_cache[n - 1];
}
