/*
 * A170555  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of reduced words of length n in the Coxeter group
 *            (S_i)^2 = (S_i S_j)^47 = I (18 generators). The sequence
 *            satisfies the linear recurrence a(0) = 1, for 1 ≤ n ≤ 46: a(n) =
 *            2 + 16 * (a(n-1)+...+a(0)), a(47) = 16*(a(46)+...+a(1)) -
 *            136*a(0), for n ≥ 48: a(n) = 16*(a(n-1)+...+a(n-46))...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache for previously computed values (thread‑unsafe). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries */

    /* If we need more space, grow the cache. */
    if (n + 1 > cache_len) {
        size_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure – return 0 */
        cache = tmp;
        /* Compute new values sequentially. */
        for (idx_t i = old_len; i < cache_len; ++i) {
            if (i == 0) {               /* a(0) = 1 */
                cache[i] = 1;
                continue;
            }
            if (i <= 46) {              /* 1 ≤ i ≤ 46 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= i; ++j)
                    sum += 16 * cache[i - j];   /* 16 * a[i-j] */
                cache[i] = 2 + sum;        /* a(i) = 2 + 16 sum_{k=0}^{i-1} a(k) */
            } else if (i == 47) {        /* i = 47 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += 16 * cache[47 - j];
                sum -= 136 * cache[0];
                cache[i] = sum;           /* recurrence for n = 47 */
            } else {                    /* i ≥ 48 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += 16 * cache[i - j];
                sum -= 136 * cache[i - 47];
                cache[i] = sum;           /* recurrence for n ≥ 48 */
            }
        }
    }
    return cache[n];
}
