/*
 * A023779  Metadromes: digits in base 11 are in strict ascending order.
 * https://oeis.org/A023779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 617, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: full, pass, 1024 terms through n = 1024 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates numbers whose base‑11 digits are strictly increasing.
   For n≥1 it returns the nth such number (OEIS A023779).  The sequence
   contains 1024 terms; for n>1024 this function returns 0. */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recursive helper that builds all numbers of a given length L. */
static void build_len(int L, int pos, int start, seq_t val, size_t *idx, seq_t *cache)
{
    if (pos == L) {
        (*idx)++;
        cache[*idx] = val;
        return;
    }
    int max_digit = 10 - (L - pos - 1);          /* digits left to place */
    for (int d = start; d <= max_digit; ++d) {
        build_len(L, pos + 1, d + 1, val * 11 + (seq_t)d, idx, cache);
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (!cache) { /* first call – build the whole sequence */
        cache_size = 1024 + 1;               /* index 0 unused */
        cache = mem_new(seq_t, cache_size);
        if (!cache) return 0;

        size_t idx = 0;

        /* length 1: digits 0..10 */
        for (int d = 0; d <= 10; ++d) {
            cache[++idx] = (seq_t)d;
        }

        /* lengths 2..10 : only digits 1..10 are allowed */
        for (int L = 2; L <= 10; ++L) {
            build_len(L, 0, 1, (seq_t)0, &idx, cache);
        }
    }

    if (n >= 1 && (size_t)n < cache_size)
        return cache[n];
    return 0;
}
