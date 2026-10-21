/*
 * A225501  Expansion of 1/(1 - x^4 - x^5 - x^6 - x^7 - x^8 + x^12).
 * https://oeis.org/A225501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)-a(n-12) (with a(0)=1,
 *            a(1)=a(2)=a(3)=0)
 * Range:     u8 n <= 26, u16 n <= 46, u32 n <= 86, u64 n <= 167,
 *            u128 n <= 329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed_up_to = 0;

    /* Initialize on first call */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cap = 1;
        computed_up_to = 0;
    }

    /* Ensure enough space for up to index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) {
            new_cap = new_cap == 0 ? 1 : new_cap * 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cap; i < new_cap; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Fill cache up to n if needed */
    if (n > computed_up_to) {
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            seq_t sum = 0;
            if (k >= 4) sum += cache[k - 4];
            if (k >= 5) sum += cache[k - 5];
            if (k >= 6) sum += cache[k - 6];
            if (k >= 7) sum += cache[k - 7];
            if (k >= 8) sum += cache[k - 8];
            if (k >= 12) sum -= cache[k - 12];
            cache[k] = sum;
        }
        computed_up_to = n;
    }

    return cache[n];
}
