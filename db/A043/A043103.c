/*
 * A043103  Numbers k such that 1 and 2 occur juxtaposed in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A043103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043103
   Numbers k such that the base‑4 representation of k contains an adjacent
   digit pair 1,2 or 2,1, but the base‑4 representation of k‑1 does not.
   The function searches incrementally and memoises the found terms. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x contains the consecutive digits 1,2 or 2,1 in its base‑4 representation. */
static bool has12(seq_t x)
{
    seq_t prev = 4;                 /* sentinel larger than any base‑4 digit */
    while (x != 0) {
        seq_t curr = x % 4;         /* least‑significant digit */
        if ((prev == 2 && curr == 1) || (prev == 1 && curr == 2))
            return true;            /* found the pattern 12 or 21 */
        prev = curr;
        x = x / 4;
    }
    return false;
}

/* Static state between calls */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t   last_checked = 0;   /* last k examined */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise the cache if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* produce terms until the requested index is available */
    while (cache_size < (size_t)n) {
        ++last_checked;
        if (has12((seq_t)last_checked) && !has12((seq_t)(last_checked - 1))) {
            /* grow the cache if needed */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)(n - 1)];
}
