/*
 * A168974  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n<23 a(n)=2+7∑_{k=1}^{n}a(n−k); for n≥23
 *            a(n)=7∑_{k=1}^{22}a(n−k)−28a(n−23)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of previously computed a(i) */
    static idx_t cached = 0;         /* highest index computed so far + 1 */

    if (n < cached)                 /* already known */
        return cache[n];

    /* enlarge the cache to hold terms up to n */
    size_t newsize = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)                       /* out of memory – return 0 safely */
        return 0;
    cache = tmp;

    /* compute missing terms from current 'cached' up to 'n' */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* sum of the last up to 22 terms */
        idx_t kmax = i < 22 ? i : 22;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i < 23) {          /* 1 ≤ i ≤ 22: linear part of the GF */
            cache[i] = (seq_t)2 + (seq_t)7 * sum;
        } else {                      /* i ≥ 23: recurrence from the rational GF */
            cache[i] = (seq_t)7 * sum - (seq_t)28 * cache[i - 23];
        }
    }

    cached = n + 1;
    return cache[n];
}
