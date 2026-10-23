/*
 * A180053  a(1)=1, a(2)=101, a(n) = 1001*a(n-1) for n > 2.
 * https://oeis.org/A180053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 101, a(n) = 1001 · a(n-1) for n > 2 (OEIS
 *            A180053)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute a(n) for n ≥ 1.
   The function is memoised; thus subsequent calls for the same or
   larger n are fast. */
seq_t a(idx_t n)
{
    /* static cache 1‑indexed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* number of allocated slots */
    static idx_t last   = 0;      /* highest n already computed */

    /* Return cached value if possible */
    if (n <= last)
        return cache[n];

    /* Ensure we have space for indices 1 … n */
    size_t needed = (size_t)n + 1; /* +1 because we ignore index 0 */
    if (needed > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache = new_cache;
        cap   = needed;
    }

    /* Compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)101;
        } else {
            cache[i] = cache[i-1] * (seq_t)1001;
        }
    }

    last = n;
    return cache[n];
}
