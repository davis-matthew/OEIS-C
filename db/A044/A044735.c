/*
 * A044735  Numbers n such that string 2,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044735
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

/* 
   Compute the nth term of OEIS A044735: numbers n such that
   the decimal representation of n contains the substring "22"
   but the decimal representation of n+1 does not.
   The function builds the list lazily, caching results to
   avoid repeated work. 
*/

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether decimal representation of x contains "22" */
static bool has22(seq_t x)
{
    uint8_t prev = 10;          /* a digit outside 0-9 */
    while (x != (seq_t)0) {
        uint8_t d = (uint8_t)(x % (seq_t)10);
        if (d == 2 && prev == 2) return true;
        prev = d;
        x /= (seq_t)10;
    }
    return false;
}

/* Lazy cache of sequence values */
static seq_t *cache = NULL;         /* cached values */
static size_t cache_len = 0;        /* number of values stored */
static size_t cache_cap = 0;        /* allocated capacity */
static seq_t last_checked = (seq_t)0;   /* last integer examined */

/* Ensure that at least 'n' terms are stored in the cache */
static void ensure_cache(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* Next candidate */
        seq_t m = last_checked + (seq_t)1;
        last_checked = m;

        if (has22(m) && !has22(m + (seq_t)1)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) {             /* offset is 1; n=0 is undefined */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[(size_t)(n - (idx_t)1)];
}
