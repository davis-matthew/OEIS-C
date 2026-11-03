/*
 * A044136  Numbers n such that string 5,4 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044136: numbers n such that the base‑6 representation of n contains the adjacent digits 5,4 while n‑1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* true iff x's base‑6 representation contains the substring “5,4” */
static bool has_54(seq_t x) {
    if (x == 0) return false;
    seq_t prev = 0;
    seq_t cur;
    bool first = true;
    while (x != 0) {
        cur = x % 6;
        if (!first && cur == 5 && prev == 4) return true;
        prev = cur;
        first = false;
        x = x / 6;
    }
    return false;
}

static seq_t *cache = NULL;      /* stored a(1)…a(k) */
static idx_t cache_cnt = 0;      /* number of terms stored */
static idx_t cache_cap = 0;      /* current capacity of cache array */
static idx_t next_candidate = 1; /* next integer to test */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    if (!cache) {
        cache_cap   = 16;
        cache_cnt   = 0;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_cnt < n) {
        seq_t cur = (seq_t)next_candidate;
        if (has_54(cur) && !has_54(cur - 1)) {
            if (cache_cnt == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_cnt++] = cur;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
