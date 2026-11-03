/*
 * A039534  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 8's.
 * https://oeis.org/A039534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑12 representation contains
 *            the same number of non‑zero digits 4 and 8 (each at least once).
 *            Computation is by brute‑force search with memoised cache
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x has the same non‑zero number of 4's and 8's in base 12. */
static bool has_equal_4_8_nonzero(seq_t x)
{
    if (x == (seq_t)0) return false;
    unsigned c4 = 0, c8 = 0;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)12);  /* digit in base 12 */
        if (d == 4u) ++c4;
        else if (d == 8u) ++c8;
        x = x / (seq_t)12;
    }
    return (c4 == c8) && (c4 > 0);
}

/* Static cache of already found terms. */
static seq_t *cache     = NULL;   /* array of seq_t */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity (in seq_t elements) */
static seq_t last_checked   = 0;  /* last integer examined */

/* Ensure there is room in the cache for at least one more element. */
static void ensure_cache_space(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache)
            exit(EXIT_FAILURE);  /* allocation failed */
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not defined for offset 1, but harmless */
    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = last_checked + (seq_t)1;
        last_checked = cand;
        if (has_equal_4_8_nonzero(cand)) {
            ensure_cache_space();
            cache[cache_len++] = cand;
        }
    }
    return cache[(size_t)n - 1];
}
