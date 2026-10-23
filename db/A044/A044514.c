/*
 * A044514  Numbers n such that string 5,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑6 representation contains the
 *            adjacent digits "51" but the base‑6 representation of n+1 does
 *            not
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: true iff x (in base 6) contains the pattern 5 followed by 1
   (from the most significant side).  We detect it by scanning the
   digits from least‑significant to most‑significant, looking for a
   digit 5 immediately after a previous digit 1. */
static bool has51(seq_t x)
{
    seq_t prev = (seq_t)6;           /* value that never equals 0‑5 */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)6;      /* least significant digit */
        if (d == (seq_t)5 && prev == (seq_t)1)
            return true;
        prev = d;
        x /= (seq_t)6;
    }
    return false;
}

/* External linkage: returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = (seq_t)0;   /* number last examined */

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = last_checked + (seq_t)1;
        last_checked = cand;

        if (has51(cand) && !has51(cand + (seq_t)1)) {
            /* Ensure space in cache. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
