/*
 * A044205  Numbers n such that string 2,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044205:
   numbers n such that the base‑8 representation of n contains the
   substring "22" while the representation of n‑1 does not.
   The function searches for the n‑th such number. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a base‑8 representation of x contains the substring "22". */
static bool has22(seq_t x)
{
    seq_t prev = 8;                /* impossible digit (outside 0–7) */
    while (x) {
        seq_t d = x % 8;
        if (d == 2 && prev == 2) return true;
        prev = d;
        x /= 8;
    }
    return false;
}

/* Cached list of already found terms. */
static seq_t *cache = NULL;
static idx_t  cache_len   = 0;
static idx_t  cache_cap   = 0;
static idx_t  last_checked = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n=0 */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        for (idx_t cand = last_checked; ; ++cand) {
            seq_t val = cand;
            if (has22(val) && !has22(val - 1)) {
                /* Allocate more space if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);   /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                last_checked = cand + 1;
                break;
            }
        }
    }
    return cache[n - 1];
}
