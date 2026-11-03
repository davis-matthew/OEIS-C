/*
 * A044722  Numbers n such that string 0,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements R. 71485791: a(n) is the n‑th positive integer whose
 * decimal representation contains the substring "09" but the next
 * integer n+1 does not.  The sequence is indexed with a(1)=109. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff the decimal representation of x contains the
 * contiguous digits 0 followed by 9. */
static bool has_09(seq_t x)
{
    /* A single digit cannot contain a two‑digit substring. */
    if (x < (seq_t)10)
        return false;

    /* Extract the least significant digit first.  The array
     * holds digits in least‑significant‑first order, so a pair
     * of consecutive digits satisfying d_{i+1}=0 and d_i=9
     * appears as previous == 9 && current == 0. */
    seq_t prev = x % (seq_t)10;
    x /= (seq_t)10;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;   /* next less significant digit */
        if (prev == (seq_t)9 && cur == (seq_t)0)
            return true;
        prev = cur;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the n-th term (n >= 1).  Calls may arrive in arbitrary order. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of found terms      */
    static size_t cache_cap  = 0;      /* allocated capacity        */
    static size_t cache_used = 0;      /* number of terms stored    */
    static seq_t next_candidate = (seq_t)1; /* integer being tested    */

    if (n == 0)      /* undefined for offset 1; return 0 safely */
        return (seq_t)0;

    /* Ensure the cache can hold at least n terms.  Grow exponentially
     * to keep allocations O(log n). */
    while (cache_used < (size_t)n) {
        /* Grow cache if necessary */
        if (cache_used == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();                /* allocation failure   */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate */
        if (has_09(next_candidate) && !has_09(next_candidate + (seq_t)1))
            cache[cache_used++] = next_candidate;

        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
