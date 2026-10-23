/*
 * A045100  Numbers whose base-4 representation contains exactly three 1's and
 *          no 2's.
 * https://oeis.org/A045100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a simple incremental search for numbers whose base‑4
   representation contains exactly three 1’s and no 2’s. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_valid(seq_t x)
{
    idx_t ones = 0;
    while (x > 0) {
        int digit = (int)(x % 4);
        if (digit == 2) return false;
        if (digit == 1) ++ones;
        x = x / 4;
    }
    return ones == 3;
}

seq_t a(idx_t n)
{
    /* Static cache of already generated terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms stored */
    static idx_t cache_cap  = 0;    /* allocated capacity */
    static seq_t next_start = 0;    /* next integer to test */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        /* Grow cache if necessary */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (cache == NULL) abort();
        }

        /* Find next valid number */
        while (!is_valid(next_start))
            next_start = next_start + 1;

        cache[cache_size] = next_start;
        ++cache_size;
        next_start = next_start + 1;
    }

    return cache[n - 1];
}
