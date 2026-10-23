/*
 * A032992  If d,e are consecutive digits of n in base 10, then |d-e|>=4.
 * https://oeis.org/A032992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 2906, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth member of the sequence A032992: the sequence of
   positive integers whose consecutive decimal digits differ by at
   least 4.
   The function enumerates integers in increasing order and caches
   the results in static storage. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: check whether all consecutive decimal digits of x differ
   by at least 4.  The single‑digit numbers trivially satisfy the
   condition. */
static bool
is_valid(seq_t x)
{
    if (x < ((seq_t)10))
        return true;

    int prev = (int)(x % 10);
    x /= 10;
    while (x > 0) {
        int cur = (int)(x % 10);
        int diff = prev - cur;
        if (diff < 0)
            diff = -diff;
        if (diff < 4)
            return false;
        prev = cur;
        x /= 10;
    }
    return true;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n < 1, but return 0 safely */

    static idx_t stored = 0;          /* number of terms cached so far */
    static seq_t last_checked = 0;    /* last integer examined */
    static seq_t *cache = NULL;       /* pointer to cached terms */
    static size_t cache_cap = 0;      /* capacity of the cache */

    /* Generate terms until we have at least n of them. */
    while (stored < n) {
        last_checked++;                       /* next candidate number */

        if (is_valid(last_checked)) {
            /* Grow the cache if necessary. */
            if (stored == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real project we might handle allocation
                       failure differently. For this isolated function
                       exiting is acceptable. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[stored++] = last_checked;
        }
    }

    /* Return the desired term.  Stored indices are zero‑based. */
    return cache[(size_t)(n - 1)];
}
