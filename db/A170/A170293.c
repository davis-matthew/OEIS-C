/*
 * A170293  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1: a(n) = p(n) + 42·Σ_{k=1}^{40} a(n‑k) –
 *            903·a(n‑41), where p(0)=1, p(1)…p(40)=2, p(41)=1, and p(n)=0 for
 *            n>41. This follows from the rational generating function given
 *            in OEIS A170293
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static idx_t last  = (idx_t)-1;   /* no values computed yet */

    /* Allocate or grow the cache as needed. */
    if (cache == NULL) {
        cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        last = (idx_t)-1;               /* nothing computed yet */
    } else if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        /* copy already computed values */
        size_t copy_cnt = (last == (idx_t)-1) ? 0 : (size_t)(last + 1);
        for (size_t i = 0; i < copy_cnt; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap = new_cap;
        /* last remains unchanged */
    }

    /* Compute values from (last+1) up to n. */
    idx_t start = (last == (idx_t)-1) ? 0 : last + 1;

    for (idx_t i = start; i <= n; ++i) {
        seq_t p = 0;
        if (i == 0 || i == 41) p = (seq_t)1;
        else if (i <= 40) p = (seq_t)2;

        seq_t sum = 0;
        for (idx_t j = 1; j <= 40 && i >= j; ++j) {
            sum += cache[i - j] * (seq_t)42;
        }

        seq_t sub = 0;
        if (i >= 41) sub = cache[i - 41] * (seq_t)903;

        cache[i] = p + sum - sub;
    }

    last = n;
    return cache[n];
}
