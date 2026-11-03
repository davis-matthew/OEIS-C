/*
 * A151946  a(1) = 8127; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 8127; a(n) = kaprekar(a(n‑1)) for n > 1
   Kaprekar step: sort the decimal digits of x in descending order,
   sort them in ascending order, subtract: desc − asc.
   Handles arbitrary-width decimal representation (no leading zeros). */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Perform one Kaprekar step on x. */
static seq_t kaprekar_step(seq_t x)
{
    uint8_t digit_buf[128];  /* plenty for any practical width */
    size_t dlen = 0;
    seq_t tmp = x;

    /* Extract digits (least significant first) */
    if (tmp == 0) {
        digit_buf[0] = 0;
        dlen = 1;
    } else {
        while (tmp > 0) {
            digit_buf[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Sort digits ascending (bubble sort is fine for dlen ≤ 128) */
    for (size_t i = 0; i + 1 < dlen; ++i) {
        for (size_t j = i + 1; j < dlen; ++j) {
            if (digit_buf[j] < digit_buf[i]) {
                uint8_t t = digit_buf[i];
                digit_buf[i] = digit_buf[j];
                digit_buf[j] = t;
            }
        }
    }

    /* Build ascending number */
    seq_t asc = 0;
    for (size_t i = 0; i < dlen; ++i) {
        asc = asc * 10 + digit_buf[i];
    }

    /* Build descending number (reverse of ascending) */
    seq_t desc = 0;
    for (size_t i = dlen; i > 0; --i) {
        desc = desc * 10 + digit_buf[i - 1];
    }

    return desc - asc;
}

/* internal cache for already computed terms */
static seq_t *cache   = NULL;   /* array of a(i) */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* out of range */

    /* Grow cache if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;            /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first term if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)8127;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = kaprekar_step(cache[i - 1]);
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
