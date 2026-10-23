/*
 * A044768  Numbers n such that string 5,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044768: numbers n whose decimal representation contains
 * the substring "55" but the representation of n+1 does not.
 * The implementation performs a brute‑force scan generating terms on
 * demand. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Check whether the decimal representation of x contains the
 * consecutive digits 5,5.  Works for any seq_t that supports the
 * required arithmetic operators. */
static bool contains55(seq_t x)
{
    const seq_t TEN = 10;
    seq_t prev = x % TEN;
    seq_t cur  = x / TEN;
    while (cur > 0) {
        seq_t d = cur % TEN;
        if (d == prev && prev == 5)
            return true;
        prev = d;
        cur  = cur / TEN;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache of already generated terms (1‑based index) */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t last_checked = 0;   /* last integer examined */

    if (n == 0)  /* offset is 1; undefined for n==0, but return 0 */
        return 0;

    /* Extend the cache until we have the required term. */
    while (cache_size < n) {
        last_checked += 1;          /* next candidate to test */
        seq_t cand = last_checked;
        if (contains55(cand) && !contains55(cand + 1)) {
            /* append cand to the cache */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = alloc_seq(new_cap);
                if (cache != NULL) {
                    for (size_t i = 0; i < cache_size; ++i)
                        new_cache[i] = cache[i];
                    free_seq(cache);
                }
                cache   = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 1];   /* zero‑based array access, 1‑based API */
}
