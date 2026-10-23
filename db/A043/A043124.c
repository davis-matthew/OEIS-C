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
 * Helpers:   grow_seq, has15or51
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043124 – numbers k such that 1 and 5 occur juxtaposed in
 * the base‑6 representation of k but not of k-1.
 * The function scans successive integers, keeping a cache of
 * found terms; it works for arbitrary‑size seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/has15or51.h"

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
