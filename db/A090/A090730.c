/*
 * A090730  a(n) = 22*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 22.
 * https://oeis.org/A090730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 22*a(n-1) - a(n-2), a(0) = 2, a(1) = 22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 703 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t   c_len  = 0;   /* number of elements filled */
    static idx_t   c_cap  = 0;   /* allocated capacity */

    /* Ensure the cache can hold index n */
    if (n >= c_cap) {
        idx_t new_cap = c_cap ? (c_cap << 1) : 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = mem_resize(seq_t, cache, (size_t)new_cap);
        c_cap = new_cap;
    }

    /* Initialise base cases if not already done */
    if (c_len == 0) {
        cache[0] = (seq_t)2;
        c_len = 1;
    }
    if (c_len < 2) {
        cache[1] = (seq_t)22;
        c_len = 2;
    }

    /* Compute terms up to n */
    for (idx_t i = c_len; i <= n; ++i) {
        if (i < 2) continue;   /* base cases already set */
        seq_t prev      = cache[i-1];
        seq_t prevprev  = cache[i-2];
        seq_t result;
        /* Use 128‑bit intermediate for built‑in types up to 64 bits */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 temp = (unsigned __int128)prev * 22u;
            result = (seq_t)temp - prevprev;
        } else {
            result = ((seq_t)22 * prev) - prevprev;
        }
        cache[i] = result;
    }
    c_len = n + 1;
    return cache[n];
}
