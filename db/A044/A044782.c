/*
 * A044782  Numbers n such that string 6,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal representation of n contains
   the substring "69" but the decimal representation of n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper: does x contain the substring "69" in its decimal representation? */
static bool has_69(seq_t x)
{
    if (x < 10) return false;      /* fewer than two digits → impossible */

    seq_t n = x;
    seq_t pow = 1;

    /* Find the highest power of 10 <= n */
    while (pow <= n)
    {
        pow *= 10;
    }
    pow /= 10;   /* now pow is 10^(digits-1) */

    int prev = -1;

    /* Scan digits from most significant to least significant */
    while (pow > 0)
    {
        int digit = (int)(n / pow);
        n %= pow;
        if (prev == 6 && digit == 9) return true;
        prev = digit;
        pow /= 10;
    }
    return false;
}

/* Cached sequence values and generation state */
static seq_t *cache      = NULL;
static size_t cache_size = 0;      /* number of stored terms   */
static size_t cache_cap  = 0;      /* capacity of allocated array */
static seq_t candidate   = 1;      /* next number to test */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for the offset, but safe fallback */

    /* Grow the cache if necessary */
    while (cache_size < (size_t)n)
    {
        /* Ensure we have space for the next term */
        if (cache_cap == 0)       /* first allocation */
        {
            cache_cap = 64;
            cache = mem_new(seq_t, cache_cap);
            if (!cache) return 0; /* allocation failure, unlikely */
        }
        else if (cache_size == cache_cap)
        {
            cache_cap <<= 1;          /* double capacity */
            cache = mem_resize(seq_t, cache, cache_cap);
            if (!cache) return 0;     /* allocation failure */
        }

        if (has_69(candidate) && !has_69(candidate + 1))
        {
            cache[cache_size++] = candidate;
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
