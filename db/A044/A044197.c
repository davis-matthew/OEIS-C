/*
 * A044197  Numbers n such that string 1,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that base‑8 representation of k contains
 *            "12" but not that of k‑1. Implementation: brute‑force scan with
 *            memoisation
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* true if base‑8 digits of x contain the substring 1,2 */
static inline bool has12_base8(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    seq_t digs[128];
    int d = 0;
    const seq_t base8 = (seq_t)8;

    while (x != (seq_t)0) {
        digs[d++] = x % base8;
        x = x / base8;
    }
    for (int i = 0; i + 1 < d; ++i) {
        if (digs[i] == (seq_t)2 && digs[i + 1] == (seq_t)1)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 1; /* next number to test */

    /* Grow the cache until we have at least n terms */
    while (cache_size < n) {
        /* Ensure there is room for another term */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)               /* allocation failed */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t cur = (seq_t)next_candidate;
        bool cur_has = has12_base8(cur);
        bool prev_has = has12_base8((seq_t)(next_candidate - 1));
        if (cur_has && !prev_has)
            cache[cache_size++] = cur;

        ++next_candidate;
    }

    return cache[n - 1];
}
