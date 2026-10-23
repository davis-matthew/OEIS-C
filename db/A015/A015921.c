/*
 * A015921  Positive integers n such that 2^n == 4 (mod n).
 * https://oeis.org/A015921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A015921: the nth integer m such that 2^m ≡ 4 (mod m),
   by incremental search. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;

    while (cache_size < (size_t)n) {
        last_checked++;
        seq_t rem = mod_pow((seq_t)2, (uint64_t)last_checked, (seq_t)last_checked);
        uint64_t rhs_u = 4ULL % (uint64_t)last_checked;
        seq_t rhs = (seq_t)rhs_u;
        if (rem == rhs) {
            if (cache_size == cache_cap) {
                grow_seq(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 4);
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n-1];
}
