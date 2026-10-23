/*
 * A044730  Numbers n such that string 1,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose decimal representation contains "17"
   but the next integer (n+1) does not. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/contains17.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 16;      /* start searching from 17 */

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the offset, safe return */
    while (cache_size < (size_t)n) {
        for (;;) {
            ++last_checked;
            seq_t x = last_checked;
            if (contains17(x) && !contains17(x + (seq_t)1)) {
                ensure_cap(cache_size + 1);
                cache[cache_size++] = x;
                break;
            }
        }
    }
    return cache[n - 1];
}
