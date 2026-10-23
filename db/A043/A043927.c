/*
 * A043927  Numbers k such that 3 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑7 representation of
 *            k contains adjacent digits 3 and 5 (in any order) but the base‑7
 *            representation of k+1 does not. The function searches
 *            sequentially, caching results so that any ordering of calls
 *            works
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has35_or53_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has35_or53_base7.h"

/* Return the n‑th term of sequence A043927 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, safety */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 1;          /* next integer to test */

    while (cache_len < (size_t)n) {
        seq_t cur = next_k;
        bool ok = has35_or53_base7(cur) && !has35_or53_base7(cur + (seq_t)1);
        if (ok) {
            if (cache_len == cache_cap) {   /* grow cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++next_k;
    }
    return cache[n - 1];
}
