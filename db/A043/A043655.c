/*
 * A043655  Numbers whose base-12 representation has exactly 6 runs.
 * https://oeis.org/A043655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A043655: Numbers whose base-12 representation has exactly 6 runs.
   The algorithm scans natural numbers, counts runs in base 12,
   and keeps a static cache of the found numbers.  For each call it
   extends the cache until it contains the n-th element. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of found numbers */
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* current capacity of the array */
static seq_t next_candidate = 1; /* next number to test */

/* Count the number of runs in the base‑12 representation of x.
   A run is a maximal block of identical digits. */
static unsigned count_runs(seq_t x)
{
    unsigned runs = 0;
    int prev_digit = -1;
    while (x != 0) {
        int d = (int)(x % 12);
        if (d != prev_digit) {
            ++runs;
            prev_digit = d;
        }
        x /= 12;
    }
    return runs;
}

/* Return the n‑th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n elements. */
    while (cache_len < n) {
        /* Ensure there is room for a new element. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* Out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the next candidate. */
        if (count_runs(next_candidate) == 6) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
