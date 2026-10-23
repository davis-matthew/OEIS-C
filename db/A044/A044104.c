/*
 * A044104  Numbers k such that string 0,2 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the base‑6 representation of
 *            k contains the substring "02" but the base‑6 representation of
 *            k‑1 does not. The function searches sequentially and caches
 *            results for later calls
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_sub02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_sub02.h"

/* Cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t   next_k    = 1;   /* next integer to test */

/* Ensure the cache has capacity for at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < need)
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Generate terms until the cache holds at least 'requested' terms. */
static void generate_up_to(idx_t requested)
{
    if (requested <= cache_len)
        return;
    ensure_cache((size_t)requested);
    while (cache_len < (size_t)requested) {
        if (has_sub02((seq_t)next_k) && !has_sub02((seq_t)(next_k - 1))) {
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for offset 1 */
    generate_up_to(n);
    return cache[n - 1];
}
