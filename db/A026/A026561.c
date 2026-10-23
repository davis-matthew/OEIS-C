/*
 * A026561  Expansion of 1/((1-2x)(1-6x)(1-8x)(1-9x)).
 * https://oeis.org/A026561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=25, a(2)=405, a(3)=5405
   a(n)=25*a(n‑1)-220*a(n‑2)+780*a(n‑3)-864*a(n‑4) for n>=4. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;       /* largest index computed, inclusive */
    static bool initialized = false;

    /* First call: allocate and initialize cache */
    if (!initialized) {
        size_t cap = (n > 3 ? n + 1 : 4);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;     /* out of memory */
        cache[0] = 1;
        if (cap > 1) cache[1] = 25;
        if (cap > 2) cache[2] = 405;
        if (cap > 3) cache[3] = 5405;
        cache_cap = cap;
        computed = 3;
        initialized = true;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t term1 = cache[i - 1] * 25;
            seq_t term2 = cache[i - 3] * 780;
            seq_t pos   = term1 + term2;
            seq_t term3 = cache[i - 2] * 220;
            seq_t term4 = cache[i - 4] * 864;
            seq_t neg   = term3 + term4;
            cache[i] = pos - neg;     /* pos > neg for all n >= 4 */
        }
        computed = n;
    }

    return cache[n];
}
