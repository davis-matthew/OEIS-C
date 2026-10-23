/*
 * A044461  Numbers n such that string 0,3 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer n whose base‑5 representation contains
 *            the consecutive digits 0 and 3, but whose successor n+1 does not
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* return true iff x has the substring "03" in its base‑5 digits */
static bool has_03(seq_t x)
{
    seq_t prev = 5;          /* sentinel (never a valid base‑5 digit) */
    while (x > 0) {
        seq_t cur = x % 5;   /* least‑significant digit */
        if (prev == 3 && cur == 0)
            return true;
        prev = cur;
        x = x / 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of cached terms */

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        /* start searching from the next integer after the last cached term */
        seq_t candidate = (len == 0) ? 1 : cache[len - 1] + 1;

        /* find the next integer satisfying the condition */
        while (true) {
            if (has_03(candidate) && !has_03(candidate + 1))
                break;
            candidate = candidate + 1;
        }

        /* grow the cache if needed */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = candidate;
    }

    return cache[n - 1];
}
