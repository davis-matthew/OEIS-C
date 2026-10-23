/*
 * A044710  Numbers n such that string 8,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer >0 whose base‑9 representation contains
 *            the adjacent digits 8,6, while the base‑9 representation of n+1
 *            does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true if the base‑9 representation of x contains the
   substring 86 (i.e. a 8 followed by 6). */
static bool has86(seq_t x)
{
    if (x == 0) return false;

    seq_t prev = x % 9;   /* least significant digit */
    x /= 9;
    while (x > 0) {
        seq_t curr = x % 9;   /* next higher digit */
        if (prev == 6 && curr == 8) return true;
        prev = curr;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  next = 1;          /* next integer to test */

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n) {
        seq_t val = (seq_t)next;

        if (has86(val) && !has86(val + 1)) {
            /* Ensure enough space. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (cache) {
                    for (size_t i = 0; i < cache_size; ++i)
                        new_cache[i] = cache[i];
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
        ++next;
    }

    return cache[n - 1];
}
