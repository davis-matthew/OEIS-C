/*
 * A044703  Numbers n such that string 7,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains78, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/contains78.h"

/* Computes a(n): the nth integer whose base‑9 representation contains
   the substring "78" while the following integer does not. */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 0;   /* last integer examined */

    if (n == 0) return 0;  /* undefined for offset 1 */

    while (cache_len < n) {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        if (contains78(cand) && !contains78(cand + 1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
