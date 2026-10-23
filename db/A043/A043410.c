/*
 * A043410  Numbers whose base-7 representation contains exactly two 4's.
 * https://oeis.org/A043410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑7 representation contains exactly two 4's */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_two_fours.h"

/* Internal state for incremental generation */
static seq_t *cache        = NULL;  /* array of found terms */
static size_t cache_cap    = 0;     /* allocated capacity */
static size_t cache_len    = 0;     /* number of terms stored */
static seq_t  last_checked = (seq_t)0;  /* last number examined */

/* Allocate or grow the cache to hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    /* realloc returns void* in C++; cast to seq_t* here */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* fatal on allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

/* External function that returns the n‑th term (n >= 1) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* offset 1; behavior below offset is irrelevant */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (has_two_fours(last_checked)) {
            ensure_capacity(++cache_len);
            cache[cache_len - 1] = last_checked;
        }
    }

    return cache[n - 1];
}
