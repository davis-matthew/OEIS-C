/*
 * A033098  Number of 2's when k is written in base b for all b and k
 *          satisfying 2<=b<=n+1, 1<=k<=n.
 * https://oeis.org/A033098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=1..n} Σ_{b=2..k+1} count_2(k,b) where count_2(k,b)
 *            is the number of times the digit '2' appears in the base‑b
 *            representation of k. This implementation builds a cache so that
 *            each a(n) is computed in O(n²) time the first time it is
 *            requested, and in O(1) thereafter. It handles arbitrar...
 * Range:     u8 n <= 39, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last = 0;        /* maximal n already cached */
    if (n == 0) return 0;         /* offset 1: a(1) will be cached as cache[1] */

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = 0;                  /* a(0) is not used but convenient */
        last = 0;
    } else if (n > last) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return cache[last]; /* cannot grow, return current */
        cache = tmp;
    }

    /* Compute values for any new indices */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t sum_k = 0;
        for (idx_t b = 2; b <= k + 1; ++b) {
            idx_t tmp = k;
            while (tmp) {
                if (tmp % b == 2) sum_k += (seq_t)1;
                tmp /= b;
            }
        }
        cache[k] = cache[k - 1] + sum_k;
    }

    last = n;
    return cache[n];
}
