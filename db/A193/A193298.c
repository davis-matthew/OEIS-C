/*
 * A193298  Gica-Panaitopol recursion: a(1) = 1; a(n+1) = 2*a(n) if a(n) <= n;
 *          otherwise a(n+1) = a(n) - 1.
 * https://oeis.org/A193298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1; a(n+1)=2*a(n) if a(n)<=n else a(n)-1 */
/* Sequence A193298 – Gica–Panaitopol recursion */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* 1‑based cache */
    static idx_t cap = 0;
    static idx_t computed = 0;    /* largest n whose value is cached */

    if (n < 1) return 0;          /* undefined for n<1, guard */

    /* Ensure storage for indices up to n */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? (n + 1) : cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Fill in missing values */
    if (n > computed) {
        if (computed == 0) {
            cache[1] = (seq_t)1;
            computed = 1;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prev = cache[i-1];
            if (prev <= (seq_t)(i-1))
                cache[i] = prev * (seq_t)2;
            else
                cache[i] = prev - (seq_t)1;
        }
        computed = n;
    }

    return cache[n];
}
