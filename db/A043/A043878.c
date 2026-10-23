/*
 * A043878  Numbers k such that 0 and 2 occur juxtaposed in the base-3
 *          representation of k but not of k+1.
 * https://oeis.org/A043878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_adjacent_02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th integer k (k>=1) whose base‑3 representation
   contains at least one adjacent pair of digits 0 and 2,
   while the base‑3 representation of k+1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_adjacent_02.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static seq_t next_start_k = 1;   /* smallest k not yet examined */

    if (n == 0)
        return 0;                    /* offset starts at 1 */

    while (cache_sz < n) {
        /* Grow cache if necessary */
        if (cache_sz >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (seq_t k = next_start_k; ; ++k) {
            if (has_adjacent_02(k) && !has_adjacent_02(k + 1)) {
                cache[cache_sz++] = k;
                if (cache_sz == n) {
                    next_start_k = k + 1;
                    break;
                }
            }
        }
    }

    return cache[n - 1];
}
