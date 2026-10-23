/*
 * A043243  Numbers k such that 3 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043243: numbers k such that 3 and 7 occur juxtaposed
   in the base-10 representation of k but not of k-1. */

#include <stdlib.h>
#include <stdbool.h>

/* check whether a decimal number contains the adjacent digits 37 or 73 */
static bool has_adj(seq_t x)
{
    seq_t y = x;
    int last = -1;            /* previous (higher) digit, -1 means none yet */
    while (y > 0) {
        int digit = (int)(y % 10);
        if (last == -1) {
            last = digit;
        } else {
            if ((last == 7 && digit == 3) || (last == 3 && digit == 7))
                return true;
            last = digit;
        }
        y /= 10;
    }
    return false;
}

static seq_t *cache = NULL;     /* stored terms of the sequence */
static size_t cache_len = 0;    /* number of filled elements */
static size_t cache_cap = 0;    /* allocated capacity */

seq_t a(idx_t n)
{
    /* offset: a(1) is first term, so for n<1 the result is undefined */
    if (n == 0) return 0;

    while (cache_len < n) {
        /* start searching after the last computed term */
        seq_t candidate = (cache_len == 0) ? 37 : cache[cache_len - 1] + 1;

        /* find the next valid number */
        for ( ; ; candidate++) {
            if (has_adj(candidate) && !has_adj(candidate - 1)) {
                /* store it */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = candidate;
                break;
            }
        }
    }
    return cache[n - 1];
}
