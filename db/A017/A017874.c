/*
 * A017874  Expansion of 1/(1-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16).
 * https://oeis.org/A017874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, …, a(7)=0, a(8)=1, …, a(15)=1,
 *            a(n)=a(n‑8)+a(n‑9)+a(n‑10)+a(n‑11)+a(n‑12)+a(n‑13)+a(n‑14)+a(n‑15)+a(n‑16)
 *            for n ≥ 16
 * Range:     u8 n <= 40, u16 n <= 69, u32 n <= 127, u64 n <= 241,
 *            u128 n <= 471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size (in elements) */
    static idx_t  filled = 0;   /* number of values already computed */

    /* enlarge storage if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        /* zero newly allocated elements */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values */
    for (idx_t i = filled; i <= n; ++i) {
        if (i == 0)            cache[i] = 1;
        else if (i < 8)        cache[i] = 0;
        else if (i < 16)       cache[i] = 1;
        else {
            seq_t sum = 0;
            for (int k = 8; k <= 16; ++k) sum += cache[i - k];
            cache[i] = sum;
        }
    }
    filled = n + 1;
    return cache[n];
}
