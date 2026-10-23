/*
 * A045051  Numbers whose base-4 representation contains exactly two 0's and
 *          four 2's.
 * https://oeis.org/A045051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1420, u32 n <= 9650, u64 n <= 9650,
 *            u128 n <= 9650, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search for numbers whose base-4 representation has exactly two 0's
    and exactly four 2's.  Numbers are generated in increasing order
    and stored in a static cache that is grown as needed.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool has_pattern(idx_t value)
{
    uint64_t x = (uint64_t)value;      // idx_t is defined as unsigned 64‑bit
    int zero = 0, two = 0;
    while (x) {
        int d = x % 4;
        x /= 4;
        if (d == 0) {
            if (++zero > 2) return false;
        } else if (d == 2) {
            if (++two > 4) return false;
        }
    }
    return (zero == 2 && two == 4);
}

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    /* Convert the requested 1‑based index to 0‑based array index. */
    if (n == 0) return (seq_t)0;  /* unused; defined for safety */

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < (size_t)n) {
        /* Locate the next integer that satisfies the pattern. */
        while (!has_pattern(next_candidate))
            ++next_candidate;

        /* Ensure the cache has enough capacity. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }
        /* Store the found number. */
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;          /* move to the following integer */
    }
    return cache[n - 1];            /* convert from 1‑based to 0‑based */
}
