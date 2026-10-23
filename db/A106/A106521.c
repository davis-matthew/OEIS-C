/*
 * A106521  Numbers m such that Sum_{k=0..10} (m+k)^2 is a square.
 * https://oeis.org/A106521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A198949(n+1)-5; a(1)=18, a(2)=38, a(3)=456, a(4)=854; for
 *            n≥5: a(n) = 20*a(n‑2) – a(n‑4) + 90 (the recurrence uses
 *            a(1)‑based indexing)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t last = 0;      /* highest index computed so far (1‑based) */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)realloc(NULL, cap * sizeof(seq_t));
        /* initial values: a(1) … a(4) */
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)38;
        cache[2] = (seq_t)456;
        cache[3] = (seq_t)854;
        last = 4;
    }

    if (n <= last)
        return cache[n - 1];

    /* extend cache up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* ensure capacity */
        if (i > cap) {
            idx_t newcap = cap * 2;
            if (newcap < i) newcap = i;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }

        seq_t val;
        /* recurrence: a(i) = 20*a(i-2) - a(i-4) + 90, for i>=5 */
        if (i >= 5) {
            val = (seq_t)20 * cache[i - 3] - cache[i - 5] + (seq_t)90;
        } else {
            /* should not happen, but keep safe */
            val = 0;
        }
        cache[i - 1] = val;
    }
    last = n;
    return cache[n - 1];
}
