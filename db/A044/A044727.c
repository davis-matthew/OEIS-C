/*
 * A044727  Numbers n such that string 1,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th term of OEIS A044727:  
   the sequence of numbers n such that the decimal representation
   of n contains the substring "14" while the representation of n+1
   does not.  It uses a brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* cache of already found terms */
static seq_t *cache = NULL;       /* array of terms */
static size_t cache_cap = 0;     /* allocated space in cache */
static size_t cache_size = 0;    /* number of terms stored */
static seq_t last_checked = 0;   /* largest integer examined so far */

/* return true if x contains the substring "14" in its decimal representation */
static bool has14(seq_t x)
{
    seq_t t = x;
    int prev = -1;                       /* digit to the right of the current one */
    while (t > 0)
    {
        int digit = (int)(t % 10);        /* least significant decimal digit */
        if (prev == 4 && digit == 1)      /* pattern 1 followed by 4 */
            return true;
        prev = digit;                     /* next iteration will see the next digit to the left */
        t /= 10;
    }
    return false;
}

/* enlarge the cache array so that it can hold at least |needed| elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_p)
        abort();   /* allocation failure */
    cache = new_p;
    cache_cap = new_cap;
}

/* Compute the n-th term of the sequence (n ≥ 1).  For n == 0 we return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless */

    /* Return cached value if it already exists */
    if (cache_size >= n)
        return cache[n - 1];

    /* Ensure the cache has space for the requested number of terms */
    grow_cache(n);

    /* Search for new terms starting after the last integer we examined. */
    seq_t candidate = last_checked + 1;
    while (cache_size < n)
    {
        if (has14(candidate) && !has14(candidate + 1))
            cache[cache_size++] = candidate;
        candidate++;
    }

    last_checked = candidate - 1;
    return cache[n - 1];
}
