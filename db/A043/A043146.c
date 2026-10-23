/*
 * A043146  Numbers k such that 3 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj34_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043146: numbers k such that the base‑7 representation of k contains two
   adjacent digits 3 and 4 (in either order), and the representation of
   k‑1 does not. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj34_local.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;              /* next integer to test */

    while (cache_size < (size_t)n) {
        seq_t val  = (seq_t)next_k;
        seq_t prev = val - 1;

        if (has_adj34_local(val) && !has_adj34_local(prev)) {
            /* Append val to cache */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_arr) abort();
                for (size_t i = 0; i < cache_size; ++i) new_arr[i] = cache[i];
                if (cache) free(cache);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
