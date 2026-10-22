/*
 * A003635  Inconsummate numbers in base 10: no number is this multiple of the
 *          sum of its digits (in base 10).
 * https://oeis.org/A003635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   search: k is a term iff no s >= 1 has digitsum(k*s) = s; since s
 *            <= 9*digits(k*s), only s <= 9*(digits(k)+3) needs checking
 * Range:     u8 n <= 10, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   dsum, is_inconsummate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/dsum.h"
#include "../utils/is_inconsummate.h"

static uint64_t *cache = NULL;
static idx_t cache_len = 0, cache_cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n) {
    while (cache_len < n) {
        uint64_t k = next_k++;
        if (is_inconsummate(k)) {
            if (cache_len == cache_cap) {
                idx_t nc = cache_cap ? cache_cap * 2 : 1024;
                cache = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                cache_cap = nc;
            }
            cache[cache_len++] = k;
        }
    }
    return (seq_t)cache[n - 1];
}
