/*
 * A043970  Numbers k such that 1 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adj14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043970: Numbers k such that 1 and 4 occur juxtaposed in the base‑9
 * representation of k but not of k+1.
 * This implementation searches for such numbers, memoising all
 * found values so that any index can be requested in O(1) after
 * an initial linear scan up to that index. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj14.h"

/* ------------------------------------------------------------------ */
/* Static state for memoisation                                            */
static seq_t *cache = NULL;      /* array of found terms                 */
static idx_t cache_len = 0;      /* number of terms stored               */
static idx_t cache_cap = 0;      /* current allocated capacity           */
static seq_t next_k = 1;         /* next integer to test                 */

/* ------------------------------------------------------------------ */
/* Ensure that the cache can hold at least 'new_cap' elements.          */
static void ensure_cache_cap(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    idx_t new_size = (cache_cap == 0) ? 128 : cache_cap;
    while (new_size < new_cap)
        new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) /* allocation failed – keep the old cache */
        return;

    cache = tmp;
    cache_cap = new_size;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Entry point – OEIS sequence generator                                 */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n == 0; caller should use n >= 1 */
        return 0;

    /* Find new terms until we have computed at least n of them. */
    while (cache_len < n) {
        /* Search the next integer that fulfills the definition. */
        while (true) {
            if (has_adj14(next_k) && !has_adj14(next_k + 1))
                break;
            ++next_k;
        }

        /* Store the found number in the cache. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = next_k;
        ++next_k;          /* next search will start from the following integer */
    }

    return cache[n - 1];
}
