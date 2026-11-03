/*
 * A033063  Numbers whose base-12 representation Sum_{i=0..m} d(i)*12^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 982 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033063: Numbers whose base‑12 representation  Σ d(i)*12^i has odd d(m‑i) for all odd i. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool seq_is_valid(seq_t x)
{
    seq_t val = x;
    uint8_t *digits = NULL;
    size_t len = 0, cap = 0;

    if (val == 0)
        return true;       /* not used for n ≥ 1 */

    /* Extract digits in base‑12 */
    while (val > 0) {
        if (len == cap) {
            cap = cap ? cap << 1 : 8;
            digits = (uint8_t *)realloc(digits, cap);
        }
        seq_t rem = val % 12;
        digits[len++] = (uint8_t)rem;
        val = val / 12;
    }

    /* m = len‑1 */
    size_t m = len - 1;
    for (size_t i = 1; i <= m; i += 2) {
        size_t j = m - i;
        if ((digits[j] & 1) == 0) {
            free(digits);
            return false;
        }
    }

    free(digits);
    return true;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n < 1 */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate/check cache */
    if (cache == NULL) {
        cache_cap = 256;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    }

    seq_t val;
    if (cache_len == 0)
        val = (seq_t)1;
    else
        val = cache[cache_len - 1] + 1;

    /* Generate terms until we reach n */
    while (cache_len < n) {
        if (seq_is_valid(val)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = val;
        }
        val = val + 1;
    }

    return cache[n - 1];
}
