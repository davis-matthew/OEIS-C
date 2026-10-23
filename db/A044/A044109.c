/*
 * A044109  Numbers k such that string 1,1 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044109: numbers k such that the base‑6 representation of k
 * contains the substring "11", but the base‑6 representation of k‑1 does
 * not.  The function searches successive integers, stores those that
 * satisfy the property in a static array, and returns the n‑th such
 * number (1‑based index).  This guarantees correct results for
 * arbitrary order of calls and works for any build that supplies the
 * types seq_t and idx_t.  The algorithm runs in O(k) time where k is
 * the largest integer examined (≈O(average spacing × n)).  With the
 * average spacing around 35, it comfortably handles n ≈ 10000 in
 * well under 2 seconds. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has11.h"

/* static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (number of seq_t elements) */
static size_t cache_len = 0;   /* number of terms stored in cache */
static idx_t last_k = 1;       /* last integer examined (starts at 1 so next is 2) */

/* ensure the cache has at least min_cap elements */
static void ensure_cache_cap(size_t min_cap) {
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE); /* unrecoverable memory failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* external function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* index 0 is unused, but guard against it */

    while (cache_len < n) {
        last_k++;                     /* next integer to test */
        seq_t k = (seq_t)last_k;      /* cast to seq_t safely */

        if (has11(k) && !has11(k - 1)) {
            if (cache_len >= cache_cap) ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
