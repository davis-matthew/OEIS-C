/*
 * A028764  Nonsquares mod 51.
 * https://oeis.org/A028764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: full, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool
is_nonsquare(idx_t x)
{
    /* mod 3: only 2 is a nonresidue */
    unsigned r3 = (unsigned)(x % 3);
    if (r3 == 2)
        return true;

    /* mod 17: nonresidues are 3,5,6,7,10,11,12,14 */
    unsigned r17 = (unsigned)(x % 17);
    switch (r17) {
        case 3: case 5: case 6: case 7:
        case 10: case 11: case 12: case 14:
            return true;
        default:
            return false;
    }
}

/* Sequence: list of positive integers that are quadratic nonresidues modulo 51
   (i.e., 3·17). */

static seq_t *
ensure_cache_capacity(size_t needed, size_t *cap, seq_t **cache)
{
    if (needed <= *cap)
        return *cache;

    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, *cache, new_cap);
    if (!tmp) abort();          /* allocation failed */

    *cache = tmp;
    *cap   = new_cap;
    return *cache;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t next_candidate = 1;   /* next integer to test */

    /* make sure we have at least n entries */
    cache = ensure_cache_capacity((size_t)n, &cache_cap, &cache);

    while (cache_len < (size_t)n) {
        if (is_nonsquare(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
