/*
 * A011673  A binary m-sequence: expansion of reciprocal of x^6+x^5+1.
 * https://oeis.org/A011673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) XOR a(n-5) for n ≥ 6 Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1 This generates the binary
 *            m‑sequence expansion of 1/(x^6+x^5+1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of elements currently cached */

    /* First call – allocate the initial 6 terms */
    if (cache == NULL) {
        cap   = 6;                       /* indices 0..5 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – give up */

        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = 0;
        cache[5] = 1;
    }

    /* Expand the cache if necessary */
    size_t needed = (size_t)n + 1;            /* we need indices 0..n */
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed – give up */
        cache = tmp;

        /* Compute new terms using the recurrence */
        size_t start = cap;
        if (start < 6) start = 6;            /* ensure we start at 6 */
        for (size_t i = start; i < needed; ++i) {
            cache[i] = cache[i-6] ^ cache[i-5];
        }

        cap = needed;
    }

    return cache[(size_t)n];
}
