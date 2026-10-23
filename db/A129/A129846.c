/*
 * A129846  Numbers k such that k and 3*k share at least one digit.
 * https://oeis.org/A129846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) is the nth integer k such that k and 3*k share
   at least one decimal digit (OEIS A129846). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute a bitmask for all decimal digits present in v. */
static inline uint16_t digit_mask(seq_t v)
{
    uint16_t mask = 0;
    if (v == (seq_t)0)
        return 1;            /* digit 0 present */
    while (v != (seq_t)0) {
        seq_t d = v % (seq_t)10;
        mask |= (uint16_t)(1 << (int)d);
        v /= (seq_t)10;
    }
    return mask;
}

/* Cache of computed sequence values (0‑based index). */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static seq_t next_k = (seq_t)1;

/* Ensure the cache has at least min_cap elements. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_capacity >= min_cap)
        return;
    size_t new_cap = cache_capacity ? cache_capacity : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined for n < 1 */

    /* Generate terms until we have at least n values. */
    while (cache_size < (size_t)n) {
        seq_t k = next_k;
        seq_t m = k * ((seq_t)3);
        if (digit_mask(k) & digit_mask(m)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
