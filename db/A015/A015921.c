/*
 * A015921  Positive integers n such that 2^n == 4 (mod n).
 * https://oeis.org/A015921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 871 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A015921: the nth integer m such that 2^m ≡ 4 (mod m),
   by incremental search. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;

    while (cache_size < (size_t)n) {
        last_checked++;
        seq_t rem = powmod((seq_t)2, (uint64_t)last_checked, (seq_t)last_checked);
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
