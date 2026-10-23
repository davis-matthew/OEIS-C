/*
 * A044222  Numbers n such that string 4,3 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Numbers n such that substring "43" occurs in the base‑8 representation
 * of n but not of n-1.  a(1) = 35, a(2) = 99, … */
static seq_t *cache = NULL;       /* cached sequence values          */
static size_t cache_size = 0;     /* number of cached values         */
static size_t cache_cap = 0;      /* allocated capacity of cache     */
static seq_t next_candidate = 1;  /* next integer to start search    */

/* Return true if the octal representation of x contains the consecutive
 * digits 4 followed by 3 (i.e., the substring "43"). */
static bool has_43(seq_t x)
{
    int prev = -1;                       /* previous (less significant) digit */
    while (x > 0) {
        int digit = (int)(x % (seq_t)8);
        /* At this point prev is the digit that follows digit in the
           octal string (more significant).  We detect "...43..." when
           digit==4 (more significant) and prev==3 (less significant). */
        if (digit == 4 && prev == 3)
            return true;
        prev = digit;
        x = x / (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; undefined for n==0 */

    /* Grow the cache until we have the nth term. */
    while (cache_size < n) {
        seq_t v = next_candidate;
        for (;;) {
            /* v must satisfy condition on itself and on v-1. */
            if (has_43(v) && !has_43(v - (seq_t)1)) {
                /* Cache this value. */
                if (cache_size >= cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = v;
                next_candidate = v + (seq_t)1;
                break;
            }
            v = v + (seq_t)1;
        }
    }
    return cache[n - 1];
}
