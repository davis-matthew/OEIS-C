/*
 * A044357  Numbers n such that string 2,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the substring "25" occurs in the decimal
 * representation of n but not in the decimal representation of n-1. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the decimal representation of x contains "25". */
static inline bool contains25(seq_t x)
{
    /* Scan the digits from least‑to‑most. */
    while (x >= 10) {
        seq_t d0 = x % 10;   /* last digit */
        x /= 10;
        seq_t d1 = x % 10;   /* preceding digit */
        if (d1 == 2 && d0 == 5)
            return true;
    }
    return false;
}

/* Return the n‑th term of A044357.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* Lazy cache of already‑computed terms. */
    static seq_t *cache        = NULL;
    static size_t cache_len    = 0;
    static size_t cache_cap    = 0;
    static seq_t last_checked  = 25;  /* next integer to test */

    if (n == 0)      /* undefined; return a harmless value */
        return 0;

    /* Allocate the cache on first use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        /* Find the next integer that satisfies the condition. */
        while (1) {
            if (contains25(last_checked) && !contains25(last_checked - 1)) {
                /* Store the term.  Expand the cache if required. */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = last_checked;
                last_checked++;            /* advance for the next search */
                break;
            }
            last_checked++;
        }
    }

    return cache[n - 1];
}
