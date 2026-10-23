/*
 * A044746  Numbers n such that string 3,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 647, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044746: numbers n such that the base‑10 representation of n
   contains the substring "33" but that of n+1 does not.
   Computed by iterating through integers and testing this condition. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x contains the consecutive digits 3,3 in its decimal
   representation. */
static bool has33(seq_t x)
{
    int prev = -1;
    while (x > (seq_t)0)
    {
        int d = (int)(x % (seq_t)10);
        if (d == 3 && prev == 3)
            return true;
        prev = d;
        x /= (seq_t)10;
    }
    return false;
}

/* Dynamically sized cache of already computed terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */
static seq_t last_checked = (seq_t)32;   /* maximum number examined so far */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache    = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 – undefined for 0 */
        return (seq_t)0;

    /* Generate terms until we have the nth one. */
    while (cache_len < n)
    {
        seq_t cur = last_checked + (seq_t)1;
        if (has33(cur) && !has33(cur + (seq_t)1))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = cur;
        }
        last_checked = cur;
    }
    return cache[n - 1];
}
