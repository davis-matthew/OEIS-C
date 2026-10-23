/*
 * A121032  Multiples of 12 containing a 12 in their decimal representation.
 * https://oeis.org/A121032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121032: multiples of 12 whose decimal representation contains
   the substring "12".  The function searches multiples of 12 in order and
   memoises the results so arbitrary order of calls is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: true if decimal digits of x contain the substring 12 */
static bool contains12(seq_t x)
{
    /* Scan from least significant digit to most significant. In reverse
       order the substring "12" becomes "21"; we detect prev==2 && cur==1. */
    seq_t prev = 10;          /* value that cannot be a decimal digit */
    while (x != 0) {
        seq_t cur = x % 10;
        if (prev == 2 && cur == 1)
            return true;
        prev = cur;
        x = x / 10;
    }
    return false;
}

/* Cache of previously found terms to handle out‑of‑order requests. */
static seq_t *cache      = NULL;   /* dynamically allocated array. */
static idx_t cache_size = 0;       /* number of terms stored. */

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for OEIS offset 1, but guard anyway */
        return 0;

    /* Trivial cache hit. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Dynamically grow the cache until we have at least n terms. */
    seq_t *new_cache = cache;
    idx_t cap = cache_size;

    /* Find terms until we reach n. */
    idx_t i   = 1;            /* current multiple index of 12. */
    idx_t seen = 0;           /* how many terms have been found. */

    /* If we already have some cache, resume scanning from the next untouched
       multiple. Use last cached term's multiple index to continue. */
    if (cache_size > 0) {
        /* The last cached term corresponds to some multiple of 12. We cannot
           deduce the exact multiple index from its value in a direct way
           for arbitrary seq_t types, so we restart scanning from i = 1
           and skip over already cached terms. */
        i = 1;
        seen = 0;
    }

    while (seen < n) {
        seq_t m = 12 * (seq_t)i;    /* multiple of 12 */
        if (contains12(m)) {
            /* Append to cache if needed. */
            if (seen >= cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); abort(); }
                cache = tmp;
                cap = new_cap;
            }
            cache[seen] = m;
            seen++;
        }
        i++;
    }

    cache_size = seen;
    return cache[n - 1];
}
