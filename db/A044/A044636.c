/*
 * A044636  Numbers n such that string 0,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer n such that the base‑9 representation of n
 *            contains the substring "04" but the representation of n+1 does
 *            not
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a deterministic search and memoises
   the result so that later calls are fast.  It works for any build
   where seq_t and idx_t are integral types or an arbitrary‑precision
   integer class with the usual arithmetic operators. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool has04(seq_t x)
{
    bool first = true;
    seq_t prev = (seq_t)0;
    while (x > (seq_t)0)
    {
        seq_t digit = x % (seq_t)9;
        if (!first && digit == (seq_t)0 && prev == (seq_t)4)
            return true;
        first = false;
        prev = digit;
        x /= (seq_t)9;
    }
    return false;
}

static seq_t *cache = NULL;      /* cached terms */
static idx_t cache_size = 0;     /* number of terms cached */
static idx_t cache_capacity = 0; /* allocated capacity */
static seq_t candidate = (seq_t)1;/* next candidate to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for 0, guard */

    while (cache_size < n)
    {
        seq_t cur = candidate;
        candidate += (seq_t)1;

        if (has04(cur) && !has04(cur + (seq_t)1))
        {
            if (cache_capacity == 0)
            {
                cache_capacity = 64;
                cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
            }
            else if (cache_size >= cache_capacity)
            {
                cache_capacity *= 2;
                cache = (seq_t*)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = cur;
        }
    }
    return cache[n - 1];
}
