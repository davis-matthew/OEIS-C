/*
 * A044843  Positive integers having no fewer base-2 runs of even length than
 *          odd.
 * https://oeis.org/A044843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: positive integers having no fewer base‑2 runs of even length
 * than odd.  The function searches integers, tests the run condition,
 * and memoises the results. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number has #even‑length binary runs >= #odd‑length runs. */
static bool has_balanced_runs(seq_t x)
{
    if (x == 0) return false;              /* zero is not allowed */
    seq_t temp = x;
    int prev_bit = (int)(temp & 1);         /* least significant bit */
    int len = 1;                             /* run length */
    int even = 0, odd = 0;
    temp >>= 1;
    while (temp != 0) {
        int bit = (int)(temp & 1);
        if (bit == prev_bit) {
            ++len;
        } else {
            /* finish current run */
            if ((len & 1) == 0) ++even; else ++odd;
            prev_bit = bit;
            len = 1;
        }
        temp >>= 1;
    }
    /* record final run */
    if ((len & 1) == 0) ++even; else ++odd;
    return even >= odd;
}

/* --- static cache ----------------------------------------------------- */
static seq_t *cache = NULL;     /* cached sequence values           */
static size_t cache_cap = 0;    /* current allocation capacity      */
static size_t cache_len = 0;    /* number of values stored          */
static idx_t last_checked = 0;  /* last integer examined            */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap > 0 ? cache_cap * 2 : 16);
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();          /* allocation failure – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* --- public interface -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0 */
    /* Build cache up to index n */
    while (cache_len < (size_t)n) {
        ++last_checked;                      /* candidate integer */
        if (has_balanced_runs((seq_t)last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
