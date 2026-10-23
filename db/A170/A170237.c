/*
 * A170237  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * The sequence satisfies the linear recurrence * * a(n) =
 *            34*a(n-1) + 34*a(n-2) + ... + 34*a(n-39) - 595*a(n-40) for n ≥
 *            40 * * with the initial values a(0)=1, a(1)=36, a(2)=1260,
 *            a(3)=44100, … computed * from the rational generating function *
 *            * A(t) = (1 + 2t + 2t² + … + 2t³⁹ + t⁴⁰) / (1 - 34t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed values   */
    static size_t cache_cap = 0;    /* allocated capacity         */
    static idx_t  cache_len = 0;    /* number of values computed (0..len-1) */

    /* Ensure the cache covers up to index n */
    if (n >= cache_len) {
        /* Grow the cache if necessary */
        size_t needed = n + 1;              /* number of elements required */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }          /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise from scratch if this is the first call */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;            /* a(0) = 1 */
            cache_len = 1;
        }

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 40) {
                /* For 0 < i < 40 use the expansion formula
                 * a(i) = p(i) + Σ_{j=1..i} 34 * a(i-j)
                 * with p(i) = 1 (i==0) or 2 (1 ≤ i ≤ 39)        */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= i; ++j) {
                    sum += (seq_t)34 * cache[i - j];
                }
                seq_t pval = (i == 0) ? (seq_t)1 : (seq_t)2;
                cache[i] = pval + sum;
            } else {
                /* For i ≥ 40 use the recurrence
                 * a(i) = Σ_{j=1..39} 34 * a(i-j) – 595 * a(i-40)  */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 39; ++j) {
                    sum += (seq_t)34 * cache[i - j];
                }
                cache[i] = sum - (seq_t)595 * cache[i - 40];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
