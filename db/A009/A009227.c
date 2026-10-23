/*
 * A009227  Expansion of e.g.f.: exp(sinh(x))/exp(x).
 * https://oeis.org/A009227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..⌊(n-1)/2⌋} binomial(n-1, 2k) * a(n-2k-1)
 *            for n ≥ 1 (OEIS A009227 – number of partitions of an n‑set into
 *            odd blocks > 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cache = NULL;         /* array of computed terms */
    static idx_t  last   = 0;           /* highest index computed so far */
    static size_t cap    = 0;           /* allocated capacity of cache */

    /* initialisation on first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* grow the cache to hold up to n */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n (if necessary) */
    if (n > last) {
        for (idx_t cur = last + 1; cur <= n; ++cur) {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;  /* binomial(cur-1, 0) */

            for (idx_t j = 1; j <= cur - 1; ++j) {
                /* next binomial coefficient:
                   bin = binom(cur-1, j) = binom(cur-1, j-1) * (cur-1-(j-1)) / j */
                seq_t factor = (seq_t)(cur - 1 - (j - 1));
                bin = bin * factor;
                bin = bin / (seq_t)j;

                if ((j & 1) == 0) {             /* j is even → corresponds to 2k */
                    idx_t idx = cur - j - 1;    /* = cur - 2k - 1 */
                    seq_t prod = bin * cache[idx];
                    sum += prod;
                }
            }
            cache[cur] = sum;
        }
        last = n;
    }

    return cache[n];
}
