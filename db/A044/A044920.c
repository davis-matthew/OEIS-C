/*
 * A044920  Numbers whose base-4 run lengths form a palindrome.
 * https://oeis.org/A044920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search: test base‑4 run‑lengths for palindromicity. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count the number of terms in the sequence up to the given index n. */
static bool base4_runpal(seq_t n) {
    if (n == 0) return false;          /* base case, not part of the sequence */

    /* Extract base‑4 digits (least significant first). */
    seq_t digits[1024];
    size_t dlen = 0;
    seq_t v = n;
    while (v) {
        digits[dlen++] = v % (seq_t)4;
        v /= (seq_t)4;
    }
    if (dlen == 0) return false;       /* should never happen for n>=1 */

    /* Build run lengths scanning from the most significant digit. */
    size_t i = dlen - 1;
    seq_t runs[1024];
    size_t rlen = 0;

    seq_t current_digit = digits[i];
    seq_t run_len = 1;

    for (int j = (int)i - 1; j >= 0; --j) {
        if (digits[j] == current_digit) {
            run_len++;
        } else {
            runs[rlen++] = run_len;
            current_digit = digits[j];
            run_len = 1;
        }
    }
    runs[rlen++] = run_len;             /* last run */

    /* Check palindrome property. */
    for (size_t k = 0; k < rlen / 2; ++k) {
        if (runs[k] != runs[rlen - 1 - k]) return false;
    }
    return true;
}

static seq_t *cache = NULL;        /* found terms */
static size_t cache_cap = 0;       /* allocated capacity */
static size_t cache_len = 0;       /* number of stored terms */
static seq_t next_candidate = 1;   /* next number to test */

/* Ensure cache has capacity for at least 'needed' elements. */
static void ensure_cache(size_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1024;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            mem_free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but return 0 for safe */
    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (base4_runpal(next_candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate = next_candidate + 1;
    }
    return cache[n - 1];            /* (a(1) is the first term, index starts at 1) */
}
