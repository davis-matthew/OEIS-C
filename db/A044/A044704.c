/*
 * A044704  Numbers n such that string 8,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑9 representation contains the
 *            substring “80” but whose successor n+1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has80
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has80.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;

    if (n == 0) return 0;          /* undefined for index 0 */

    while (cache_len < n) {
        seq_t val = next_candidate;
        if (has80(val) && !has80(val + (seq_t)1)) {
            if (cache_len == cache_cap) {
                /* grow cache */
                idx_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) {
                    /* allocation failure – return 0 as a last resort */
                    return 0;
                }
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
