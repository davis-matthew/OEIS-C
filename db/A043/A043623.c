/*
 * A043623  Numbers whose base-7 representation has exactly 8 runs.
 * https://oeis.org/A043623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A043623: numbers whose base‑7 representation has exactly 8 runs.
   The function generates terms on demand by sequentially testing integers and memoizing
   found values.  It works for all idx_t and seq_t types supported by the build. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: returns true if x has exactly 8 runs in base 7. */
static bool has_8_runs(seq_t x)
{
    seq_t runs = 1;              /* at least one run */
    seq_t temp = x;
    seq_t prev = temp % (seq_t)7;
    temp /= (seq_t)7;
    while (temp > (seq_t)0) {
        seq_t d = temp % (seq_t)7;
        if (d != prev)
            ++runs;
        prev = d;
        temp /= (seq_t)7;
    }
    return runs == (seq_t)8;
}

/* The public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* unused, but safe */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_candidate;

    /* initialization on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        /* start at 7^7 = 823543 */
        next_candidate = (seq_t)1;
        for (unsigned i = 0; i < 7; ++i)
            next_candidate *= (seq_t)7;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        if (has_8_runs(next_candidate)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap == 0 ? 128 : cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
