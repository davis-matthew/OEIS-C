/*
 * A046238  Cubes which are palindromes in base 7.
 * https://oeis.org/A046238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_palindrome_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Search for cubes whose base‑7 representation is a palindrome. */
/* This function builds the sequence up to the requested index, memoizing all
   terms in a static array.  The array is grown by doubling its capacity. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_palindrome_base7.h"

#if defined(__cplusplus)
  // C++ allocator for class type seq_t
  /* Nothing to include; operator new[] is available by default. */
#endif

/* Static storage for memoization */
static seq_t *cache      = NULL;
static idx_t   cache_len = 0;   /* number of terms stored */
static idx_t   cache_cap = 0;   /* capacity of cache array   */
static idx_t   next_k    = 0;   /* next integer to test      */

/* Ensure that the cache has at least 'required' slots. */
static void ensure_capacity(idx_t required)
{
#if defined(__cplusplus)
    if (required <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
    cache_cap = new_cap;
#else
    if (required <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program we would handle the allocation failure. */
        exit(1);
    }
    if (cache_len > 0)
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
#endif
}

/* Public function: returns the n-th cube (1‑based) that is a base‑7 palindrome. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n==0 is undefined */
        return (seq_t)0;

    while (cache_len < n) {
        seq_t k      = (seq_t)next_k;
        seq_t cube   = k * k * k;
        if (is_palindrome_base7(cube)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cube;
        }
        ++next_k;
    }
    return cache[n - 1];
}
