/*
 * A044066  Numbers n such that string 1,1 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044066, the index‑wise numbers n such that
   the base‑4 representation of n contains the substring 11 but
   the representation of n‑1 does not.  The function memoises
   results so that repeated calls with arbitrary n are efficient. */

#include <stdlib.h>
#include <stdbool.h>

static inline bool has11(seq_t v)
{
    seq_t x = v;
    int prev = -1;
    while (x > 0) {
        int digit = (int)(x % 4);
        if (prev == 1 && digit == 1)
            return true;
        prev = digit;
        x = x / 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Memoised cache of sequence values */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  cur_n     = 1;          /* next integer to examine */

    /* Grow the cache until it contains n terms */
    while (cache_len < n) {
        seq_t   val      = (seq_t)cur_n;
        bool    cur_has  = has11(val);
        bool    prev_has = has11((seq_t)(cur_n - 1));

        if (cur_has && !prev_has) {
            /* Ensure capacity is sufficient */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* Out of memory – undefined behaviour */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        cur_n++;
    }

    return cache[n - 1];
}
