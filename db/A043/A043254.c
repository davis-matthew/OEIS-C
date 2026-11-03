/*
 * A043254  Numbers k such that 5 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   generates the n‑th term of A043254 – numbers k containing the
 *            juxtaposed digits 5 and 9 (either 59 or 95) in their decimal
 *            representation while k-1 does not contain such a pair
 * Range:     u8 n <= 4, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9357 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has_59_95(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (prev != -1) {
            if ((prev == 5 && d == 9) || (prev == 9 && d == 5))
                return true;
        }
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_k = 0;
    static bool prev_contains = false;

    if (n == 0) return 0; /* behaviour below offset is irrelevant */

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_len < n) {
        last_k++;                                    /* next integer to test */
        bool contains = has_59_95(last_k);
        if (contains && !prev_contains) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = last_k;
        }
        prev_contains = contains;
    }

    return cache[n - 1];
}
