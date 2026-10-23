/*
 * A044233  Numbers n such that string 5,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has56_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose base‑8 representation
   contains the substring "56" but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has56_oct.h"

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* behaviour below offset is irrelevant */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;          /* last integer examined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cur = last_checked + (seq_t)1;
        bool cur_has  = has56_oct(cur);
        bool prev_has = has56_oct(cur - (seq_t)1);

        if (cur_has && !prev_has) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();             /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }

        last_checked = cur;
    }

    return cache[n - 1];
}
