/*
 * A044721  Numbers n such that string 0,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044721
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

/* Finds n-th integer whose decimal representation contains
 * the substring “08” but n+1 does not.
 * The function scans integers sequentially, caching found terms
 * so that subsequent calls work in O(1) average time. */

#include <stdbool.h>
#include <stddef.h>

#define CACHE_MAX 20000  /* must exceed the maximum n expected */

/* Helper: does x contain the substring 08? */
static inline bool has08(seq_t x)
{
    seq_t prev = 10;   /* sentinel, not a decimal digit */
    while (x > 0)
    {
        seq_t d = x % 10;
        if (prev == 8 && d == 0) return true;
        prev = d;
        x    = x / 10;
    }
    return false;
}

/* Cache of found terms */
static seq_t  cache[CACHE_MAX];
static size_t cache_len = 0;
static idx_t  last_scanned = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    /* extend cache as needed */
    while (cache_len < (size_t)n)
    {
        last_scanned++;                     /* next integer to test   */
        seq_t next = (seq_t)last_scanned;
        if (has08(next) && !has08(next + 1))
        {
            if (cache_len < CACHE_MAX)
                cache[cache_len++] = next;
            else
                /* cache overflow should never happen for expected limits */
                ;
        }
    }

    return cache[n - 1];
}
