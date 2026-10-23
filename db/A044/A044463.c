/*
 * A044463  Numbers n such that string 1,0 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer n>0 such that its base‑5 representation
 *            contains * the substring "10" but the base‑5 representation of
 *            n+1 does not
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has10_in_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has10_in_base5.h"

/* ----- static state for memoisation ------------------------------------ */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t last_checked = 0;

/* ----- helper to grow the cache --------------------------------------- */
static bool cache_ensure(size_t needed)
{
    if (needed <= cache_cap) return true;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return false;
    cache = tmp;
    cache_cap = new_cap;
    return true;
}

/* ----- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset is 1; n==0 is undefined, return 0 */
    for ( ; cache_len < (size_t)n; ++last_checked)
    {
        if (!cache_ensure(cache_len + 1)) abort();
        seq_t val = (seq_t)last_checked;
        if (has10_in_base5(val) && !has10_in_base5(val + (seq_t)1))
            cache[cache_len++] = val;
    }
    return cache[n - 1];
}
