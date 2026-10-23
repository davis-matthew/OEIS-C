/*
 * A039492  Numbers whose base-11 representation has the same nonzero number
 *          of 9's and 10's.
 * https://oeis.org/A039492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   good_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer whose base‑11 expansion contains an equal
   non‑zero number of 9's and 10's.  The function builds the list lazily
   and memoizes it. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_term.h"

/* External integer types supplied by the build system */
extern seq_t a(idx_t n); /* declaration required by the harness */

/* Static cache storage for already discovered terms */
static seq_t *cache      = NULL;     /* dynamic array of terms          */
static size_t cache_len  = 0;        /* number of terms stored           */
static size_t cache_cap  = 0;        /* capacity of the array            */

/* Next candidate to test, in decimal */
static idx_t next_checked = 109;

/* Ensure the cache can hold at least 'min_cap' elements */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)          /* fatal in all configurations */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function.  Any call order is supported because
   the results are cached. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined; return 0 as a fallback */
        return (seq_t)0;

    /* Generate terms until we have at least 'n' of them. */
    while (cache_len < n) {
        seq_t candidate = (seq_t)next_checked;
        if (good_term(candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = candidate;
        }
        ++next_checked;
    }

    /* Indexing is 1‑based in OEIS but zero‑based in the array. */
    return cache[n - 1];
}
