/*
 * A045123  Numbers whose base-4 representation contains exactly two 1's and
 *          four 3's.
 * https://oeis.org/A045123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑4 representation contains exactly two 1's and four 3's. */

/* External integer types are supplied by the build system. */
#include <stdlib.h>     /* malloc, realloc, abort */
#include <stddef.h>     /* size_t */

/* Global cache for previously generated terms. */
static seq_t *cache = NULL;          /* sorted array of found terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* Current base‑4 length to generate next. */
static idx_t next_len = 6;

/* Append a value to the cache, expanding it if necessary. */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 256 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive enumeration of L‑digit base‑4 numbers with the required
 * counts of 1's and 3's, appending them to the cache. */
static void rec(idx_t L, idx_t pos, idx_t ones_rem, idx_t threes_rem, seq_t val)
{
    /* Prune paths that cannot possibly satisfy the constraints. */
    if (L - pos < ones_rem + threes_rem) return;

    if (pos == L) {
        /* Accept if all required digits have been placed. */
        if (ones_rem == 0 && threes_rem == 0) {
            push_cache(val);
        }
        return;
    }

    /* Try all base‑4 digits. A leading zero is not allowed. */
    for (int d = 0; d < 4; ++d) {
        if (pos == 0 && d == 0) continue;          /* leading zero forbidden */
        if (d == 1 && ones_rem == 0) continue;       /* no 1's left to place */
        if (d == 3 && threes_rem == 0) continue;     /* no 3's left to place */

        idx_t new_ones   = ones_rem - (d == 1);
        idx_t new_threes = threes_rem - (d == 3);

        /* Build the number incrementally: *4 + d. */
        rec(L, pos + 1, new_ones, new_threes, val * 4 + d);
    }
}

/* Generate all numbers of a specific base‑4 length and append them to the cache. */
static void generate_len(idx_t L)
{
    rec(L, 0, 2, 4, 0);
}

/* Public API required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is offset 1; this case is not used */

    /* Generate additional lengths until we have at least n terms. */
    while (cache_len < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }
    return cache[n - 1];
}
