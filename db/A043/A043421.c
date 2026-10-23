/*
 * A043421  Numbers having one 0 in base 8.
 * https://oeis.org/A043421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _count_zeros_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having exactly one 0 in base 8.
   The function scans consecutive integers and stores those that
   contain a single zero digit in base‑8 representation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_zeros_in_base8.h"

/* Ensure the cache has room for at least need elements. */
static void _ensure_cache(size_t need, seq_t **cache,
                          size_t *len, size_t *cap)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    *cache = tmp;
    *cap = new_cap;
}

/* External function requested by the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_val = 7;           /* the largest integer already examined */

    if (n == 0)              /* undefined for index 0, but harmless */
        return 0;

    _ensure_cache((size_t)n, &cache, &cache_len, &cache_cap);

    while (cache_len < (size_t)n) {
        ++next_val;          /* next candidate integer */

        if (_count_zeros_in_base8(next_val) == 1) {
            cache[cache_len++] = next_val;
        }
    }

    return cache[n - 1];
}
