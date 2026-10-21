/*
 * A099895  XOR BINOMIAL transform of A000069 (Odious numbers).
 * https://oeis.org/A099895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⊕{ A000069(n-i) | C(n,i) mod 2 = 1 } (XOR binomial
 *            transform)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#   include <cstddef>
#endif

/* Cache for odious numbers (A000069) */
static seq_t *odious_cache = NULL;   /* array of odious numbers */
static idx_t odious_len   = 0;        /* number of cached values */
static idx_t odious_cap   = 0;        /* current capacity of the cache */
static uint64_t odious_last_candidate = 0; /* largest integer tested */

/* Expand the cache to at least new_cap elements */
static void odious_expand(idx_t new_cap)
{
#if defined(__cplusplus)
    /* C++: use operator new[] and deep copy */
    seq_t *new_arr = new seq_t[new_cap];
    if (odious_cache) {
        for (idx_t i = 0; i < odious_len; ++i)
            new_arr[i] = odious_cache[i];
        delete[] odious_cache;
    }
    odious_cache = new_arr;
#else
    /* C: use realloc */
    seq_t *new_arr = (seq_t *)realloc(odious_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* abort on allocation failure */
    odious_cache = new_arr;
#endif
    odious_cap = new_cap;
}

/* Ensure that odious_cache contains at least (n+1) values */
static void ensure_odious_len(idx_t n)
{
    if (odious_len > n) return;                 /* already sufficient */

    /* Grow the cache if necessary */
    if (odious_cap < n + 1) {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;            /* small initial size */
        odious_expand(new_cap);
    }

    /* Generate odious numbers until we have index n */
    while (odious_len <= n) {
        odious_last_candidate++;
        if (__builtin_popcountll(odious_last_candidate) & 1u) {
            odious_cache[odious_len++] = (seq_t)odious_last_candidate;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Make sure we have all odious numbers up to index n */
    ensure_odious_len(n);

    seq_t res = 0;

    /* Iterate over all subsets of the bit–mask n */
    for (idx_t mask = n; ; mask = (mask - 1) & n) {
        idx_t j = n - mask;          /* index in A000069 */
        res ^= odious_cache[j];
        if (mask == 0) break;         /* finished when mask reaches 0 */
    }

    return res;
}
