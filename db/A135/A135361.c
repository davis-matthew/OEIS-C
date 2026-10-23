/*
 * A135361  a(n) = a(n-1)^3 + 1 with a(0) = 0.
 * https://oeis.org/A135361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135361:  a(0) = 0;  a(n) = a(n-1)^3 + 1  for n ≥ 1   */
#include <stdlib.h>     /* malloc, realloc, abort */
#include <stddef.h>     /* size_t */

static seq_t *cache      = NULL;   /* cached values a(0)…a(len-1) */
static size_t cap        = 0;      /* allocated slots in cache */
static size_t len        = 0;      /* number of valid terms in cache */

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n) {
    /* Initial allocation */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;          /* a(0) */
        len = 1;
    }
    /* Grow capacity as needed (power‑of‑two growth). */
    while ((size_t)n >= cap) {
        size_t new_cap = cap << 1;
        if (new_cap == 0) new_cap = 1;  /* when cap was 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    /* Compute any missing terms up to n. */
    for (size_t k = len; k <= (size_t)n; ++k) {
        seq_t prev = cache[k - 1];
        /* prev^3 + 1 */
        seq_t prod = prev * prev * prev;
        cache[k] = prod + 1;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
