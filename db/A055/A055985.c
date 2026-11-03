/*
 * A055985  a(n+1) = a(n) converted to base 10 from base 14.
 * https://oeis.org/A055985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 10; a(n+1) = interpret the decimal digits of a(n) as a
 *            base‑14 number and convert that number back to decimal
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 21, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Convert a decimal number x, whose digits are in base‑10, by treating
   those digits as base‑14 digits and returning the resulting decimal value. */
static seq_t convert(seq_t x)
{
    seq_t result = (seq_t)0;
    seq_t pow14  = (seq_t)1;
    seq_t temp   = x;

    while (temp > (seq_t)0) {
        seq_t digit = temp % (seq_t)10;
        result += digit * pow14;
        pow14  *= (seq_t)14;
        temp   /= (seq_t)10;
    }
    return result;
}

static seq_t *cache = NULL;          /* array of stored terms          */
static idx_t cache_len = 0;          /* next index that has not been filled */
static size_t cache_cap = 0;         /* current allocated capacity       */

/* Ensure that the cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* work with size_t for array indexing – n is always small enough. */
    size_t rn = (size_t)n;

    if (cache_len > n) return;     /* already computed */

    /* Allocate or grow the storage. */
    if (cache == NULL) {
        size_t cap = 16;
        while (cap <= rn) cap <<= 1;
        cache = mem_new(seq_t, cap);
        cache_cap = cap;
    } else if (cache_cap <= rn) {
        size_t new_cap = cache_cap;
        while (new_cap <= rn) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (size_t i = (size_t)cache_len; i <= rn; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)10;
        } else {
            cache[i] = convert(cache[i - 1]);
        }
    }

    cache_len = (idx_t)(rn + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
