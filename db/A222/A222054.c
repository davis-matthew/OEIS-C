/*
 * A222054  O.g.f.: Sum_{n>=0} (n^4*x)^n/(1-n^4*x)^n * exp(-n^4*x/(1-n^4*x)) /
 *          n!.
 * https://oeis.org/A222054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binom(n-1,k-1) * S2(4*n,k) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types:
   seq_t : unsigned integer type of any size, or arbitrary‑precision class
   idx_t : unsigned integer (uint64_t) */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static idx_t max_n = 0;           /* highest n computed so far */
    static seq_t *cache = NULL;       /* array of a(0..max_n) */
    static size_t cache_cap = 0;      /* capacity of cache */

    /* static storage for Stirling numbers S2(4*m, k) */
    static seq_t *S = NULL;           /* array of Stirling values */
    static size_t S_cap = 0;          /* capacity of S array */
    static idx_t S_kmax = 0;          /* highest k currently computed (initially 0) */

    /* First call: initialise structures */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        max_n = 0;

        S_cap = 1;
        S = (seq_t *)malloc(S_cap * sizeof(seq_t));
        S[0] = (seq_t)1;             /* S2(0,0) = 1 */
        S_kmax = 0;
    }

    /* If already computed, return cached value */
    if (n <= max_n)
        return cache[n];

    /* Grow cache if needed */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Grow Stirling array if needed when increasing S_kmax */
    /* The array size depends on S_kmax, not on n directly. */
    /* It will be resized in the loop below when S_kmax grows. */

    /* Compute a(m) for all m = max_n+1 .. n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Transition from N = 4*(m-1) to N = 4*m using 4 steps */
        for (int step = 0; step < 4; ++step) {
            /* Increase kmax and ensure capacity */
            ++S_kmax;
            if ((idx_t)S_kmax >= S_cap) {
                size_t new_cap = S_cap;
                while (new_cap <= (size_t)S_kmax) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                /* initialise newly allocated entries to 0 */
                for (size_t i = S_cap; i < new_cap; ++i)
                    tmp[i] = 0;
                S = tmp;
                S_cap = new_cap;
            }

            /* Update Stirling numbers for this step */
            /* Reference: S[n+1][k] = k*S[n][k] + S[n][k-1]  */
            for (idx_t k = S_kmax; k >= 1; --k) {
                S[k] = (seq_t)k * S[k] + S[k - 1];
            }
            S[0] = 0;  /* for n > 0, S(n,0) = 0 */
        }

        /* Compute a(m) using binomial–Stirling sum */
        seq_t binom = (seq_t)1;  /* C(m-1,0) */
        seq_t sum   = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1)
                binom = binom * (seq_t)(m - k + 1) / (seq_t)(k - 1);
            sum += binom * S[k];
        }
        cache[m] = sum;
    }

    max_n = n;
    return cache[n];
}
