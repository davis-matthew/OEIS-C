/*
 * A044662  Numbers n such that string 3,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return n‑th integer whose base‑9 representation contains the
   substring "33" but that substring does not appear in the representation
   of n+1. */
#include <stdlib.h>
#include <stdbool.h>

static bool has33(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int cur = (int)(x % 9);
        if (prev == 3 && cur == 3)
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_examined = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below offset, undefined behaviour allowed */

    /* Grow cache until we have at least n terms */
    if (cache_len < (size_t)n) {
        seq_t candidate = last_examined + 1;
        while (cache_len < (size_t)n) {
            /* check if candidate satisfies the condition */
            if (has33(candidate) && !has33(candidate + 1)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_arr = (seq_t *)realloc(cache,
                                    new_cap * sizeof(seq_t));
                    if (new_arr == NULL) exit(EXIT_FAILURE);
                    cache = new_arr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
            }
            last_examined = candidate;
            ++candidate;
        }
    }

    return cache[n - 1];
}
