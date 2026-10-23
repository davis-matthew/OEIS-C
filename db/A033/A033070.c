/*
 * A033070  Numbers with the property that all pairs of consecutive base-5
 *          digits differ by 1.
 * https://oeis.org/A033070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033070: numbers whose consecutive base‑5 digits differ by 1,
   enumerated in increasing order.  The function generates all admissible
   base‑5 strings of increasing length, converts them to decimal
   values, and stores them in a static cache.  On each call a(n) it
   expands the cache until it contains at least n entries and returns the
   (n‑1)‑th element (a(1) is the first term). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache of all already generated terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t current_len = 0;     /* longest base‑5 length generated */

/* Ensure the cache has at least 'need' slots. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failure – the sequence will remain incomplete */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Depth‑first generation of admissible numbers of a given length.
   'depth' is the current depth (starting at 1), 'target' is the final
   length, 'val' is the numeric value built so far,
   'last' is the last digit (0‑4). */
static void dfs_generate(size_t depth, size_t target,
                         seq_t val, uint8_t last)
{
    if (depth == target) {
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = val;
        return;
    }

    if (last > 0) {
        dfs_generate(depth + 1, target, val * 5 + (last - 1), last - 1);
    }
    if (last < 4) {
        dfs_generate(depth + 1, target, val * 5 + (last + 1), last + 1);
    }
}

/* Generate all admissible numbers of the specified base‑5 length. */
static void generate_length(size_t len)
{
    for (uint8_t first = 1; first <= 4; ++first) {
        dfs_generate(1, len, (seq_t)first, first);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* unused index */

    /* Generate lengths until we have at least n terms. */
    while ((size_t)n > cache_len) {
        ++current_len;           /* next base‑5 length to generate */
        generate_length(current_len);
    }

    return cache[(size_t)n - 1];
}
