/*
 * A044754  Numbers n such that string 4,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer that contains the decimal
   substring "41" but whose successor does not.  The term a(1) is 41. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether x contains the substring "41" in its base‑10 representation. */
static bool has41(seq_t x)
{
    seq_t tmp = x;
    bool prev_one = false;

    while (tmp > (seq_t)0) {
        seq_t digit = tmp % (seq_t)10;            /* least significant digit */
        if (prev_one && digit == (seq_t)4)
            return true;                         /* found 4 immediately after 1 */
        prev_one = (digit == (seq_t)1);
        tmp /= (seq_t)10;
    }
    return false;
}

/* Static state used for memoisation between calls. */
static seq_t   *cache   = NULL;      /* array of previously found terms          */
static size_t   cap     = 0;          /* allocated capacity      */
static size_t   len     = 0;          /* number of elements stored  */
static seq_t    next    = (seq_t)0;   /* smallest integer not yet tested */

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n terms. */
    while (len < n) {
        /* Allocate or grow the cache when necessary.  Use an explicit
           cast (seq_t*)realloc to satisfy both C and C++,
           because seq_t may be a non‑POD type in C++. */
        if (len >= cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* Test the current candidate. */
        if (has41(next) && !has41(next + (seq_t)1)) {
            cache[len++] = next;
        }
        next++;                                /* move to next integer */
    }

    return cache[(size_t)n - 1];                 /* a(1) is cache[0] */
}
