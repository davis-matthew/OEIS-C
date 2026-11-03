/*
 * A154314  Numbers with not more than two distinct digits in ternary
 *          representation.
 * https://oeis.org/A154314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 4329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9462 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates a(n) as the n‑th integer with at most two distinct digits in its ternary representation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff x has ≤2 distinct ternary digits. */
static bool _good_ternary(seq_t x)
{
    if (x == 0)
        return true;
    unsigned mask = 0;
    seq_t y = x;
    while (y > 0) {
        unsigned digit = (unsigned)(y % 3);
        mask |= 1U << digit;
        /* three bits set means three distinct digits */
        if ((mask & 1U) && (mask & 2U) && (mask & 4U))
            return false;
        y /= 3;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Lazy cache: generated on demand, reused across calls. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;     /* number of stored terms */
    static seq_t last_val = 0;   /* last term produced */

    if (n == 0)                    /* offset starts at 1; undefined otherwise */
        return (seq_t)0;

    /* first allocation */
    if (!cache) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
        cache[0] = (seq_t)0;      /* a(1) = 0 */
        len = 1;
        last_val = (seq_t)0;
    }

    if ((idx_t)len >= n)
        return cache[n - 1];

    /* generate terms until we have the n‑th one */
    seq_t cur = last_val + (seq_t)1;
    while ((idx_t)len < n) {
        if (_good_ternary(cur)) {
            if (len == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cur;
            last_val = cur;
        }
        cur = cur + (seq_t)1;
    }

    return cache[n - 1];
}
