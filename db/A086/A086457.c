/*
 * A086457  Both n and n^2 have the same initial digit and also n and n^2 have
 *          the same final digit when expressed in base 10.
 * https://oeis.org/A086457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th non‑negative integer m such that the first and
 *            last decimal digits of m and m² are equal. Computed by linear
 *            search with memoization
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 3083, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>

/* Global cache for memoization ------------------------------------- */
static seq_t *cache           = NULL;  /* cached sequence values   */
static size_t cache_size      = 0;     /* number of cached terms   */
static size_t cache_cap       = 0;     /* allocated array capacity */
static seq_t next_candidate   = 0;     /* next integer to test     */

/* Resize the cache array if a larger capacity is required. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < min_cap)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the most significant (leading) decimal digit of x.  */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return x;
}

/* Test whether a number satisfies the property:
   its first and last decimal digits and the first and last decimal
   digits of its square are equal. */
static int good(seq_t x)
{
    seq_t d = x % (seq_t)10;                     /* last digit of x */
    /* Only digits that can occur at the end of a square are 0,1,5,6. */
    if (d != (seq_t)0 && d != (seq_t)1 &&
        d != (seq_t)5 && d != (seq_t)6)
        return 0;

    seq_t sq = x * x;                            /* x²               */
    if (sq % (seq_t)10 != d)                     /* square's last digit */
        return 0;

    return first_digit(x) == first_digit(sq);    /* compare leading digits */
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Find the next qualifying number. */
        while (!good(next_candidate))
            next_candidate++;

        /* Store it in the cache. */
        grow_cache(cache_size + 1);
        cache[cache_size++] = next_candidate;

        next_candidate++;   /* advance for the next search */
    }

    return cache[n - 1];
}
