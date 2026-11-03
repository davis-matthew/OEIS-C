/*
 * A044121  Numbers n such that string 3,1 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044121: a(n) is the nth positive integer whose base‑6
 * representation contains the substring 3,1 but the base‑6
 * representation of the previous integer does not.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the base‑6 representation of x contains the adjacent
 * digits 3 followed by 1 (reading from most‑significant to least). */
static bool has31_pattern(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* 128 digits are more than enough for all n up to 10 000 in the
       typical 64‑bit build, and plenty for the arbitrary‑precision
       build as well. */
    uint8_t digits[128];
    unsigned len = 0;
    seq_t temp = x;

    while (temp != (seq_t)0) {
        seq_t q = temp / (seq_t)6;
        seq_t r = temp - q * (seq_t)6;   /* r = temp % 6 */
        digits[len++] = (uint8_t)r;
        temp = q;
    }

    /* Scan for the pattern 3,1 in the normal (most‑significant first)
       order.  In the digits array the least‑significant digit is at
       index 0, so a pair 3,1 corresponds to digits[i]==3 && digits[i-1]==1
       for some i>0. */
    for (unsigned i = 1; i < len; ++i) {
        if (digits[i] == 3 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

/* Return the n‑th term of the sequence.  The function memoises all
   terms up to the largest index seen so far. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    /* The next integer to test for the pattern. */
    static seq_t next_candidate = (seq_t)1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t x = next_candidate;

        while (1) {
            /* Check the requirement for x and its predecessor. */
            if (has31_pattern(x) && !has31_pattern(x - (seq_t)1)) {
                /* Store the found term. */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;              /* double the capacity */
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = x;
                next_candidate = x + (seq_t)1;
                break;
            }
            ++x;
        }
    }

    return cache[n - (idx_t)1];
}
