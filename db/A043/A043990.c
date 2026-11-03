/*
 * A043990  Numbers k such that 5 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043990
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

/*  OEIS A043990 – Numbers k such that 5 and 6 occur juxtaposed (in any
    order) in the base‑9 representation of k but not in k+1.  The function
    builds the sequence lazily and caches already found values.  It does
    not assume increasing calls.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether x has the adjacent digits 5 and 6 (in any order) in base‑9. */
static bool has56or65(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    seq_t prev = x % (seq_t)9;   /* least significant digit */
    x /= (seq_t)9;
    while (x != (seq_t)0) {
        seq_t cur = x % (seq_t)9;                     /* next higher digit */
        /* pattern 56 or 65 – cur is higher digit, prev lower digit */
        if ((cur == (seq_t)5 && prev == (seq_t)6) ||
            (cur == (seq_t)6 && prev == (seq_t)5))
            return true;
        prev = cur;
        x /= (seq_t)9;
    }
    return false;
}

/* Ensure a dynamic array is large enough. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need)
            new_cap = need;
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    if (n < (idx_t)1)
        return (seq_t)0;      /* undefined for n < 1 */

    size_t target = (size_t)n;
    if (cache_len < target) {
        /* Continue searching from the last found value + 1 */
        seq_t start = (cache_len ? cache[cache_len - 1] + (seq_t)1 : (seq_t)1);
        for (seq_t k = start; cache_len < target; ++k) {
            if (has56or65(k) && !has56or65(k + (seq_t)1)) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - (idx_t)1];
}
