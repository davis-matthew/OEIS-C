/*
 * A043884  Numbers k such that 1 and 3 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043884: numbers k such that the digits 1 and 3 occur
 * side by side in the base‑4 representation of k (in either order),
 * but they do NOT occur side by side in the base‑4 representation of
 * k+1.
 *
 * This routine searches for the desired values.  It maintains a memoised
 * cache so that subsequent calls are fast and order‑independent.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if base‑4 representation of k contains the pattern
 * "13" or "31". */
static bool has13pair(idx_t k)
{
    int prev = -1;        /* less significant digit seen so far */
    while (k > 0) {
        int cur = (int)(k % 4);
        if ((prev == 3 && cur == 1) || (prev == 1 && cur == 3))
            return true;
        prev = cur;
        k /= 4;
    }
    return false;
}

/* a(n) for n >= 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached terms */
    static size_t cache_cap = 0;   /* capacity of cache */
    static size_t cache_len = 0;   /* number of terms stored */
    static idx_t next_k = 0;       /* last tested k */

    if (n == 0)           /* out of range; behaviour undefined */
        return 0;

    /* grow cache array on first use */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* find terms until we have at least n of them */
    while (cache_len < n) {
        ++next_k;
        if (has13pair(next_k) && !has13pair(next_k + 1)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }
    return cache[n - 1];
}
