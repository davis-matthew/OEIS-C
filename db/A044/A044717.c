/*
 * A044717  Numbers n such that string 0,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044717: numbers n such that decimal representation of n contains "04"
   but does not contain "04" in n+1. */

#include <stdlib.h>
#include <stdbool.h>

/* Check if decimal representation of x contains the substring "04".  */
static bool has_04(seq_t x)
{
    if (x < 10) return false;               /* single‑digit numbers cannot contain "04" */
    seq_t prev = x % 10;
    x /= 10;
    while (x != 0) {
        seq_t cur = x % 10;
        if (cur == 0 && prev == 4) return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not part of the defined offset, but safe */

    static seq_t *cache = NULL;     /* stored sequence values */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static size_t cache_size = 0;    /* number of values stored */
    static seq_t cur = 0;            /* current candidate to test */
    size_t i;

    /* Grow the cache as needed */
    while (cache_size < (size_t)n) {
        seq_t x = cur;

        if (has_04(x) && !has_04(x + 1)) {
            if (cache_cap == cache_size) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) abort();
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = x;
        }
        ++cur;
    }

    return cache[(size_t)n - 1];
}
