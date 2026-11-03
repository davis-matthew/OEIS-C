/*
 * A043977  Numbers k such that 2 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043977
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

/* Sequence A043977: numbers k such that 2 and 5 occur juxtaposed
   (in either order) in the base‑9 representation of k but not
   in that of k+1.  Computed by sequential search. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Return true iff x contains the adjacent pair 2‑5 or 5‑2 in base‑9. */
static bool has_adj25_or52(seq_t x)
{
    if (x < (seq_t)9)            /* a single digit cannot contain a pair */
        return false;

    seq_t prev = x % (seq_t)9;
    x     /= (seq_t)9;
    while (x) {
        seq_t cur = x % (seq_t)9;
        if ((prev == (seq_t)2 && cur == (seq_t)5) ||
            (prev == (seq_t)5 && cur == (seq_t)2))
            return true;
        prev = cur;
        x   /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence is generated on demand; cache already found values. */
    static seq_t *cache = NULL;     /* dynamically resized array of terms */
    static idx_t cache_used = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static seq_t next_k = (seq_t)1; /* the next integer to test */

    if (n == 0)                    /* undefined offset – return 0 */
        return (seq_t)0;

    while (cache_used < n) {
        seq_t k = next_k;

        if (has_adj25_or52(k) && !has_adj25_or52(k + (seq_t)1)) {
            /* Grow cache if necessary. */
            if (cache_used == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *newp = mem_new(seq_t, new_cap);
                if (!newp)
                    abort();           /* out of memory */
                if (cache) {
                    mem_copy(newp, cache, cache_cap * sizeof(seq_t));
                    mem_free(cache);
                }
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_used++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
