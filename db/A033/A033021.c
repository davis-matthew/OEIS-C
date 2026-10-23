/*
 * A033021  Numbers whose base-8 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 246, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th positive integer whose base‑8 representation
   consists only of runs of identical digits of length at least 2. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* cached sequence values          */
static size_t cache_len = 0;     /* number of cached terms          */
static size_t cache_cap = 0;     /* capacity of the cache array     */
static seq_t last_checked = 0;   /* last integer examined           */

/* Ensure the cache can hold at least 'min_cap' elements. */
static void ensure_capacity(size_t min_cap) {
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff x's base‑8 digits contain no run of length 1. */
static bool valid_base8(seq_t x) {
    if (x == 0) return false;             /* zero has no runs     */
    int prev_digit = -1;
    int run_len = 0;
    while (x) {
        int d = (int)(x % (seq_t)8);      /* digit in base 8      */
        if (d == prev_digit) {
            run_len++;
        } else {
            if (run_len && run_len < 2) return false;
            prev_digit = d;
            run_len = 1;
        }
        x /= (seq_t)8;
    }
    return run_len >= 2;
}

/* Public API: returns the n-th term (a(1) is the first term). */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined for n<1        */

    /* One‑time initialization of the cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);              /* allocation failure       */
        cache_len = 0;
        last_checked = 0;
    }

    /* Generate terms until we reach n. */
    while (cache_len < (size_t)n) {
        seq_t candidate = last_checked + 1;
        last_checked = candidate;
        if (valid_base8(candidate)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
