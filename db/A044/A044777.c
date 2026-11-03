/*
 * A044777  Numbers n such that string 6,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044777: numbers n such that decimal representation contains
   "64" but n+1 does not. Computation by a linear search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if x contains the adjacent digits 6 followed by 4. */
static bool contains64(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = 0;
    while (x > 0) {
        seq_t cur = x % 10;
        if (cur == 6 && prev == 4)
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms.  The cache is expanded lazily so that
       calls are independent of the order of n. */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* Grow the cache until we have at least n terms. */
    while (cache_cnt < n) {
        seq_t cur = next_candidate;
        if (contains64(cur) && !contains64(cur + 1)) {
            if (cache_cnt >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* allocation failed – abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = cur;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
