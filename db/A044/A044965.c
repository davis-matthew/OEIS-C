/*
 * A044965  Numbers with no two equally numerous base 16 digits.
 * https://oeis.org/A044965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9904 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044965
 * Numbers with no two equally numerous base‑16 digits.
 * This implementation repeatedly scans positive integers,
 * storing the ones that satisfy the digit‑frequency condition
 * in a cache up to the requested index.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for size_t */
#include "../utils/mem.h"

/* Helper to determine whether an integer has distinct non‑zero
 * base‑16 digit frequencies.  We work in idx_t (uint64_t).         */
static bool distinct_hex_digit_frequencies(idx_t v)
{
    uint8_t freq[16] = {0};

    /* Count the frequencies of each hex digit. */
    while (v != 0) {
        /* v & 0xF gives the low hex digit; v >>= 4 discards it. */
        int d = (int)(v & 0xF);
        freq[d]++;
        v >>= 4;
    }

    /* Check that all non‑zero frequencies are unique. */
    bool used[17] = {0};                 /* frequencies range 1..16 */
    for (int i = 0; i < 16; ++i) {
        if (freq[i] == 0) continue;
        if (used[freq[i]]) return false;/* duplicate frequency found */
        used[freq[i]] = true;
    }
    return true;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache of previously computed terms.  The array stores values
 * as seq_t (the output type).  It is grown dynamically on demand. */
static seq_t  *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Return the nth term (1‑based) of the sequence.  The function may
 * be called with n in any order; results are cached to avoid
 * recomputation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 0 */

    /* Grow the cache until it holds the first n terms. */
    while (cache_len < n) {
        /* Start searching from the value right after the last stored one. */
        idx_t candidate = (cache_len == 0)
                          ? 1
                          : (idx_t)cache[cache_len - 1] + 1;

        /* Find the next number that satisfies the property. */
        while (!distinct_hex_digit_frequencies(candidate))
            ++candidate;

        /* Resize the cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();            /* unrecoverable allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found term. */
        cache[cache_len++] = (seq_t)candidate;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
