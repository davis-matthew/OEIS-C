/*
 * A043140  Numbers k such that 1 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static inline bool has_adjacent_15_or_51(seq_t v)
{
    if (v == 0)
        return false;

    int prev = -1;               /* previous digit, -1 for none */
    while (v > 0) {
        unsigned d = (unsigned)(v % 7);   /* digit in base‑7 */
        if ((prev == 1 && d == 5) || (prev == 5 && d == 1))
            return true;
        prev = d;
        v /= 7;
    }
    return false;
}

/* Numbers k such that 1 and 5 occur juxtaposed in the base‑7 representation
   of k but not of k-1. */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* Cached sequence values */
    static size_t cache_cap = 0;  /* Allocated capacity */
    static size_t cache_len = 0;  /* Number of elements stored */
    static seq_t last_checked = 0;/* Largest integer examined */

    if (n == 0)                /* Off‑by‑one safety */
        return 0;

    while (cache_len < n) {
        seq_t k = last_checked + 1;          /* Candidate to test */

        if (has_adjacent_15_or_51(k)) {
            seq_t km1 = k - 1;
            if (!has_adjacent_15_or_51(km1)) {
                /* Resize cache if necessary */
                if (cache_len >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)
                        exit(EXIT_FAILURE);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;      /* Store found value */
            }
        }
        last_checked = k;
    }

    return cache[n - 1];
}
