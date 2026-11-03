/*
 * A043966  Numbers k such that 0 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043966
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

/* Calculate by brute‑force search for the first n numbers k whose
 * base‑9 representation contains the digit pair 0 and 7 (either 07 or 70)
 * and k+1 does not.  The sequence is indexed with offset 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether a number in base 9 contains an adjacent pair 0↔7. */
static bool has_adj_07(seq_t x) {
    seq_t prev = 9;              /* sentinel: no previous digit yet */
    while (x > 0) {
        seq_t cur = x % 9;
        x /= 9;
        if (prev != 9 && ((cur == 0 && prev == 7) || (cur == 7 && prev == 0)))
            return true;
        prev = cur;
    }
    return false;
}

/* a(n) gives the n-th element (n >= 1) of sequence A043966. */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;       /* array holding found elements */
    static size_t cache_cap = 0;      /* current allocated size */
    static size_t cache_len = 0;      /* number of elements stored */
    static seq_t next_k = 1;          /* next integer to test */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure sufficient space in the cache */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* Allocation error – abort gracefully (could also set errno) */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n entries */
    while (cache_len < n) {
        seq_t cand = next_k++;
        if (has_adj_07(cand) && !has_adj_07(cand + 1)) {
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
