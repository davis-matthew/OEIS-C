/*
 * A044676  Numbers n such that string 4,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has48_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) is the n‑th integer whose base‑9 representation
 * contains the substring "48" but the next integer does not. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has48_base9.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (n == 0) return 0;   /* undefined for n < 1 */

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        last_checked++;
        if (has48_base9(last_checked) && !has48_base9(last_checked + 1)) {
            /* ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* insufficient memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
