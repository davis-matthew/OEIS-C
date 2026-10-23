/*
 * A044523  Numbers n such that string 0,4 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A044523.
   It finds the n‑th positive integer whose base‑7 representation
   contains the substring "04" but the representation of n+1 does not.
   The function is memoised, so calls with arbitrary index n are fast. */

#include <stdlib.h>
#include <stdbool.h>

static bool has04(seq_t v)
{
    if (v == 0)
        return false;
    unsigned prev = 0;
    while (v != 0) {
        unsigned digit = (unsigned)(v % 7);
        if (digit == 0 && prev == 4)
            return true;
        prev = digit;
        v /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache keeps all terms found so far. */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of cached terms */
    static size_t cap  = 0;   /* capacity of the cache */
    static idx_t cur   = 0;   /* next integer to test */

    if (n == 0)                 /* the offset is 1; return 0 for safety */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (sz < n) {
        ++cur;
        seq_t cand = (seq_t)cur;
        if (has04(cand) && !has04(cand + 1)) {
            if (sz == cap) {                /* expand the buffer */
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = cand;
        }
    }
    return cache[n - 1];
}
