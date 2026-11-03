/*
 * A077408  Trajectory of 103 under the Reverse and Add! operation carried out
 *          in base 3, written in base 10.
 * https://oeis.org/A077408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 103 a(n+1) = a(n) + reverse_base3(a(n)), where
 *            reverse_base3(x) is obtained by writing x in base‑3, reversing
 *            the digit order, and interpreting the result as a base‑10 number
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute reverse_base3(x) and return x + reverse_base3(x). */
static seq_t reverse_add(seq_t x)
{
    seq_t temp = x;

    /* Collect base‑3 digits (least‑significant first). */
    size_t cap = 64;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits)
        return x;                    /* unlikely, but harmless */

    size_t len = 0;
    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            seq_t d = temp % 3;      /* digit 0,1 or 2 */
            if (len >= cap) {        /* grow buffer if needed */
                cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
                if (!tmp) {
                    free(digits);
                    return x;
                }
                digits = tmp;
            }
            digits[len++] = (uint8_t)d;
            temp /= 3;
        }
    }

    /* Build the reversed number: interpret digits in reverse order. */
    seq_t rev = 0;
    for (size_t i = 0; i < len; ++i) {
        rev = rev * 3 + digits[i];
    }

    free(digits);
    return x + rev;
}

/* Main sequence function with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_size = 1;
        cache = mem_new(seq_t, cache_size);
        cache[0] = (seq_t)103;    /* a(0) */
    }

    /* Grow cache up to the requested index. */
    while (cache_size <= n) {
        seq_t val = reverse_add(cache[cache_size - 1]);
        seq_t *new_cache = mem_resize(seq_t, cache, (cache_size + 1));
        if (!new_cache) {          /* allocation failure: keep old cache */
            break;
        }
        cache = new_cache;
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[(size_t)n];
}
