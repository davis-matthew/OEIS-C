/*
 * A043124  Numbers k such that 1 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043124 – numbers k such that 1 and 5 occur juxtaposed in
 * the base‑6 representation of k but not of k-1.
 * The function scans successive integers, keeping a cache of
 * found terms; it works for arbitrary‑size seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return true iff x contains the adjacent digits 1 and 5 in base‑6
 * (in either order). */
static bool has15or51(seq_t x)
{
    int prev = -1;
    while (x != 0) {
        int d = (int)(x % 6);
        if ((prev == 5 && d == 1) || (prev == 1 && d == 5))
            return true;
        prev = d;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_k     = 0;   /* last number examined */

    while (cache_len < (size_t)n) {
        ++last_k;
        seq_t k = (seq_t)last_k;
        if (has15or51(k) && !has15or51(k - 1)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
