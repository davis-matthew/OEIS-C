/*
 * A044557  Numbers n such that string 5,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th natural number whose base‑7 representation
 * contains the substring "53" while the next integer does not.
 * The algorithm simply tests successive integers, keeping the ones that
 * satisfy the condition in a dynamic cache until the n‑th one is reached.
 * It works for any seq_t type supported by the build.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/contains53.h"

/* True if x contains "53" but x+1 does not. */
static bool good_candidate(seq_t x)
{
    return contains53(x) && !contains53(x + 1);
}

/* Return the n-th element of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1  */

    /* Static cache holding all found terms up to the current maximum.  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;  /* next integer to examine  */

    while (cache_len < n) {
        if (good_candidate(next_candidate)) {
            if (cache_len == cache_cap) {
                /* Grow the cache.  */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_buf) return 0;           /* allocation failure  */
                if (cache) {
                    for (idx_t i = 0; i < cache_len; ++i)
                        new_buf[i] = cache[i];
                    free(cache);
                }
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
