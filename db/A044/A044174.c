/*
 * A044174  Numbers n such that string 5,1 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044174 : numbers n such that the base-7 representation of n contains the substring "51" but n-1 does not.
 * The function computes the nth term by brute‑force generation and memoization.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has51_base7(seq_t x)
{
    if (x == 0) return false;
    /* Buffer to hold base-7 digits (least significant first). */
    uint8_t buf[64];
    size_t len = 0;
    while (x > 0) {
        buf[len++] = (uint8_t)(x % 7);
        x /= 7;
    }
    /* Scan from most significant to least significant digit. */
    for (size_t i = len; i > 1; --i) {
        if (buf[i-1] == 5 && buf[i-2] == 1) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static seq_t next_value = 0;      /* last integer examined */

    if (n == 0) return 0;            /* undefined for 0, but safe fallback */

    /* Ensure cache has at least n terms. */
    while (cache_len < n) {
        /* Get next integer to test. */
        ++next_value;
        if (has51_base7(next_value) && !has51_base7(next_value - 1)) {
            /* Store the term. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) { /* allocation failure */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_value;
        }
    }
    return cache[n - 1];
}
