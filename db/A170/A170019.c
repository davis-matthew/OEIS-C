/*
 * A170019  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/9)*∑_{i=1}^{36} a(n-i) for n≥36 Initial values:
 *            a(0)=1, a(n)=10*9^(n-1) for 1≤n≤35
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;
    static int    init         = 0;

    /* Initialise the first 36 terms the first time the function is called. */
    if (!init) {
        cache = (seq_t *)calloc(36, sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;
        seq_t pow9 = (seq_t)1;
        for (idx_t i = 1; i <= 35; ++i) {
            cache[i] = (seq_t)10 * pow9;  /* 10*9^(i-1) */
            pow9 *= (seq_t)9;              /* now pow9 = 9^i for next round */
        }
        max_computed = 35;
        init = 1;
    }

    /* Extend cache and compute new terms if the caller requested a larger n. */
    if (n > max_computed) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;

        /* Compute the initial sliding‑window sum for the next term. */
        seq_t sum = 0;
        for (idx_t i = 0; i < 36; ++i)
            sum += cache[max_computed - i];   /* a[max_computed] … a[max_computed-35] */

        /* Iterate from the first missing index up to n. */
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t val = (sum * (seq_t)2) / (seq_t)9;   /* a(k) = (2/9)*sum */
            cache[k] = val;

            /* Update sliding sum for the next index. */
            sum += val;                /* add the newly computed term */
            sum -= cache[k - 36];       /* drop the oldest term a(k-36) */
        }

        max_computed = n;
    }

    return cache[n];
}
