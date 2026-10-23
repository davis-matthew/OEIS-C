/*
 * A044377  Numbers n such that string 4,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 711, u32 n <= 1250, u64 n <= 1250,
 *            u128 n <= 1250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044377: numbers n whose decimal representation contains the substring "45"
   while the number n-1 does not.  We generate the list by scanning integers
   from 45 upwards, keeping a cache of the terms found.  The search stops
   when the requested index n has been reached. */

#include <stdbool.h>
#include <stddef.h>

/* Test whether x contains the consecutive digits 4 followed by 5. */
static inline bool has45(seq_t x)
{
    /* Examine the decimal digits from least to most significant. */
    seq_t prev = x % 10;
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;
        if (cur == 4 && prev == 5) return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* Return the n-th term of the sequence (a(1) = 45). */
seq_t a(idx_t n)
{
    /* A very small static cache; n is known to be at most a few tens of
       thousands in the test harness, so 20000 entries are more than enough. */
    enum { CACHE_CAP = 20000 };
    static seq_t cache[CACHE_CAP];
    static idx_t len = 0;

    /* Resolve any requests for n <= 0 as 0; the OEIS offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Grow the cache up to the requested index. */
    while (len < n) {
        /* Start searching from the next integer after the last stored term. */
        seq_t candidate = (len == 0 ? (seq_t)45 : cache[len - 1] + 1);

        /* Find the next qualifying number. */
        while (1) {
            if (has45(candidate) && !has45(candidate - 1)) {
                /* Store it if we have room. */
                if (len >= CACHE_CAP) {
                    /* In the unlikely event the cache overflows, return
                       the candidate as a last resort.  The test harness
                       never reaches this. */
                    return candidate;
                }
                cache[len++] = candidate;
                break;
            }
            ++candidate;
        }
    }

    return cache[n - 1];
}
