/*
 * A145407  Number of Hamiltonian paths in O_6 X P_n.
 * https://oeis.org/A145407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 350*a(n-1) -22608*a(n-2) -17280*a(n-3) + 843264*a(n-4)
 *            a(1)=120, a(2)=41280, a(3)=6641952, a(4)=886927344,
 *            a(5)=105209243232
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_filled = 0;
    /* make sure the cache has room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    if (n > cache_filled) {
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            if      (i == 1) cache[i] = (seq_t)120;
            else if (i == 2) cache[i] = (seq_t)41280;
            else if (i == 3) cache[i] = (seq_t)6641952;
            else if (i == 4) cache[i] = (seq_t)886927344;
            else if (i == 5) cache[i] = (seq_t)105209243232;
            else {
                cache[i] = (seq_t)350 * cache[i-1]
                           - (seq_t)22608 * cache[i-2]
                           - (seq_t)17280 * cache[i-3]
                           + (seq_t)843264 * cache[i-4];
            }
        }
        cache_filled = n;
    }
    return cache[n];
}
