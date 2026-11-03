/*
 * A044766  Numbers n such that string 5,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044766: Numbers n such that "53" occurs in decimal
    representation of n but not of n+1.  The function finds the
    nth such number (1‑based index) by memoised sequential search.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_digits.h"

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

static bool contains53(seq_t x)
{
    /* buf stores digits LSB first; len is number of digits.  */
    uint8_t buf[200];
    size_t len = 0;
    get_digits(x, buf, &len);
    for (size_t i = len; i > 1; ) {
        i--;                                    /* most significant pair */
        if (buf[i] == 5 && buf[i-1] == 3)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for 0‑based index */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;

    while (cache_len < (size_t)n) {
        /* Find next number satisfying the condition. */
        while (true) {
            if (contains53(next_candidate) &&
                !contains53(next_candidate + (seq_t)1)) {

                /* Ensure capacity. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp)
                        abort(); /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                next_candidate++;          /* continue from next integer */
                break;
            }
            next_candidate++;
        }
    }

    return cache[(size_t)n - 1];
}
