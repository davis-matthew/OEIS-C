/*
 * A044501  Numbers n such that string 3,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base‑6 representation of n contains the
 *            adjacent digits 3 followed by 0, but n+1 does not
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has_30
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_30.h"

/* a(n) returns the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t found = 0;
    static seq_t next = 0;

    while (found < n) {
        seq_t cur = next++;
        bool cur_ok   = has_30(cur);
        bool next_ok  = has_30(cur + (seq_t)1);
        if (cur_ok && !next_ok) {
            if (found >= cache_size) {
                size_t new_cap = cache_size ? cache_size * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_size = new_cap;
            }
            cache[found++] = cur;
        }
    }
    return cache[n - 1];
}
