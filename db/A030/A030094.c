/*
 * A030094  Product of n with 666 is palindromic.
 * https://oeis.org/A030094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   palin128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030094: a(n) = the n‑th integer k such that 666*k is a
 * decimal palindrome.  The function is memoised so that calls in
 * any order are fast.  It works with all build types of seq_t, even
 * when seq_t is too small to hold the product 666*k (e.g. 8‑bit).
 * The product is computed in an unsigned __int128 and the palindrome
 * test is performed on that value. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/palin128.h"

/* Grow a static array of seq_t */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 256 : (*cap * 2);
    if (new_cap < needed) new_cap = needed;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of cache */

    /* Return from cache if already computed */
    if (cache_len > (size_t)n) return cache[(size_t)n];

    /* Start checking from the next integer after the last stored term */
    idx_t curr = (cache_len == 0) ? 0 : (idx_t)cache[(size_t)(cache_len - 1)] + 1;

    while (cache_len <= (size_t)n) {
        unsigned __int128 prod = (unsigned __int128)curr * (unsigned __int128)666;
        if (palin128(prod)) {
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = (seq_t)curr;
        }
        curr++;
    }
    return cache[(size_t)n];
}
