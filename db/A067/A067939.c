/*
 * A067939  Let rep(n) be the n-th repunit number; sequence gives values of n
 *          such that rep(n) == rep(6) (mod n).
 * https://oeis.org/A067939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   rep_mod_equals_111111_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) is the n‑th integer k such that rep(k) ≡ rep(6) (mod k). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rep_mod_equals_111111_mod.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = (idx_t)1;

    while (cache_len < n) {
        if (rep_mod_equals_111111_mod(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory – terminate */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
