/*
 * A169194  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: 
 *   For all n ≥ 0,
 *   a(n) = 35 * Σ_{k=1}^{26} a(n‑k) – 630 * a(n‑27) + p(n)
 *   where
 *       p(0) = 1,   p(1 … 26) = 2,   p(27) = 1,   p(n > 27) = 0
 *   and a(k) = 0 for k < 0.
 *
 * The first terms are:
 *   a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, …   (see OEIS A169194)
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size of cache */
    static size_t len = 0;   /* number of terms already computed (len == n+1) */

    /* grow cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1 > 2 * cap) ? (n + 1) : 2 * cap;
        if (new_cap == 0) new_cap = 128;   /* initial allocation */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* initialize first term if needed */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* compute terms from len up to n (inclusive) */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 26; ++k) {
            if (i >= k) sum += cache[i - k];
        }
        seq_t part = (seq_t)35 * sum;
        seq_t sub  = 0;
        if (i >= 27) sub = (seq_t)630 * cache[i - 27];

        seq_t p;
        if (i == 0)       p = (seq_t)1;
        else if (i <= 26) p = (seq_t)2;
        else if (i == 27) p = (seq_t)1;
        else              p = (seq_t)0;

        cache[i] = part - sub + p;
    }

    len = n + 1;          /* all terms up to n are now computed */
    return cache[n];
}
