/*
 * A044472  Numbers n such that string 2,4 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A044472 – numbers n whose base‑5 representation contains
 * the substring “24” but the base‑5 representation of n+1 does not.
 *
 * This implementation performs a linear scan, checks each integer,
 * and memoises the results in a dynamic array so that repeated calls
 * are efficient.  It works for any seq_t size defined by the build.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if x in base 5 contains the digit pair 2,4 (most‑significant → least). */
static bool has_24_base5(seq_t x)
{
    if (x == 0)
        return false;

    /* Support enough digits for the expected range (less than 128 digits). */
    uint8_t digits[128];
    size_t len = 0;

    while (x != 0) {
        digits[len++] = (uint8_t)(x % 5);
        x /= 5;
    }

    /* Scan from most significant to least for the pattern 2,4. */
    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] == 2 && digits[i - 2] == 4) {
            return true;
        }
    }
    return false;
}

/* Memoised generator of a(n).  a(1) is the first term (14). */
seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;   /* dynamic array of found terms     */
    static size_t cache_len      = 0;      /* number of stored terms           */
    static size_t cache_cap      = 0;      /* current capacity of cache        */
    static idx_t last_checked    = 0;      /* next integer to examine (exclusive) */

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)last_checked + 1;

        if (has_24_base5(candidate) && !has_24_base5(candidate + 1)) {
            /* Resize cache when full. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) {            /* allocation failed – abort */
                    cache = NULL;
                    cache_len = 0;
                    cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        last_checked++;
    }

    return cache[n - 1];
}
