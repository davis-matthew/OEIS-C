/*
 * A044282  Numbers n such that string 3,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth number whose base‑9 representation contains the
   substring "34" but the preceding number does not.
   The function computes terms incrementally and stores them in a
   static cache so that repeated calls for arbitrary indices are
   efficient.  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_34.h"

static seq_t *cache = NULL;     /* cached terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t next_to_check = 1; /* next number to test */

/* Ensure that the cache can hold at least `need` terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *newptr = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        newptr[i] = cache[i];
    delete[] cache;
    cache = newptr;
#else
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newptr;
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, return 0 for safety */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < n) {
        if (contains_34(next_to_check) && !contains_34(next_to_check - 1)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = next_to_check;
        }
        ++next_to_check;
    }
    return cache[n - 1];
}
