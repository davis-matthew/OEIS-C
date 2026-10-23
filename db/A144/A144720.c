/*
 * A144720  a(0) = 2, a(1) = 3, a(n) = 4 * a(n-1) - a(n-2).
 * https://oeis.org/A144720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(1) = 3, a(n) = 4*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    size_t idx = (size_t)n;

    /* initial allocation and base values */
    if (cap == 0) {
        /* start with space for at least two elements */
        cap = (n < 2 ? 2 : idx + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure – abort */
        cache[0] = (seq_t)2;
        if (cap > 1) cache[1] = (seq_t)3;
    }

    /* enlarge cache if necessary */
    if (idx >= cap) {
        size_t old_cap = cap;
        /* ensure new capacity is at least idx+1, but grow at least 2x to keep amortized O(1) */
        size_t new_cap = (cap << 1);
        if (new_cap <= idx) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* initialize base values if we were initially empty */
        if (old_cap == 0) {
            cache[0] = (seq_t)2;
            if (new_cap > 1) cache[1] = (seq_t)3;
        }
        /* compute new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)2;
            } else if (i == 1) {
                cache[1] = (seq_t)3;
            } else {
                cache[i] = (seq_t)4 * cache[i-1] - cache[i-2];
            }
        }
        cap = new_cap;
    }

    /* compute missing entries up to idx if we haven't already */
    /* (this handles the case when cap > idx but some entries beyond idx are missing,
       e.g., initial cap was 2 but idx is 0 or 1.) */
    for (size_t i = 0; i <= idx; ++i) {
        if (i == 0) continue;
        if (i == 1) continue;
        if (cache[i] == 0) { /* assume unused entries are zero */
            cache[i] = (seq_t)4 * cache[i-1] - cache[i-2];
        }
    }

    return cache[idx];
}
