/*
 * A170179  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For 1 ≤ n ≤ 38: a(n)=24*∑_{k=1}^n a(n-k)+2. For n=39:
 *            a(39)=24*∑_{k=1}^{38}a(39-k)-300*a(0)+1. For n≥40:
 *            a(n)=24*∑_{k=1}^{38}a(n-k)-300*a(n-39). (Coefficients come from
 *            the rational GF of A170179.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;            /* last index computed in cache */
    /* Allocate or enlarge the cache if necessary */
    if (!cache) {
        size_t required = n + 1;
        size_t new_cap = 1;
        while (new_cap < required)
            new_cap <<= 1;
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – should not happen */
        cap = new_cap;
        cache[0] = 1;             /* a(0)=1 */
        last = 0;
    } else if (cap <= (size_t)n) {
        size_t required = n + 1;
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < required)
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – should not happen */
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i <= 38 ? i : 38);
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        seq_t N;
        if (i == 0)                 N = 1;
        else if (i <= 38)           N = 2;
        else if (i == 39)           N = 1;
        else                        N = 0;

        seq_t val;
        if (i <= 38) {                       /* 1 ≤ n ≤ 38 */
            val = 24 * sum + N;
        } else if (i == 39) {                /* n = 39 */
            val = 24 * sum - 300 * cache[i - 39] + N;
        } else {                             /* n ≥ 40 */
            val = 24 * sum - 300 * cache[i - 39];
        }
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
