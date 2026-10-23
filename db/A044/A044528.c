/*
 * A044528  Numbers n such that string 1,2 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has12_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers n such that the base‑7 representation contains the digits
   "12" (a 1 immediately followed by a 2 when read from most significant
   to least significant) and that n+1 does not contain that pattern.
   Returns the nth such number (a(1)=9). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has12_in_base7.h"

/* Dynamic cache of already found terms. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cur = (seq_t)0;

/* Ensure the cache can hold at least 'needed' entries. */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* guard for safety */
    while ((idx_t)cache_len < n) {
        cur = cur + (seq_t)1;                 /* test next integer */
        if (has12_in_base7(cur) && !has12_in_base7(cur + (seq_t)1)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = cur;
        }
    }
    return cache[(size_t)n - 1];
}
