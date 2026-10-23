/*
 * A163124  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=20, a(2)=380, a(3)=7220, a(4)=136990; For n>=5:
 *            a(n) = 18*(a(n-1)+a(n-2)+a(n-3)) - 171*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t last_computed = (idx_t)-1;

    /* allocate and initialise when first called */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)380;
        cache[3] = (seq_t)7220;
        cache[4] = (seq_t)136990;
        last_computed = 4;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to n */
    if (last_computed < n) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            seq_t sum = cache[k-1] + cache[k-2] + cache[k-3];
            cache[k] = sum * (seq_t)18 - cache[k-4] * (seq_t)171;
        }
        last_computed = n;
    }

    return cache[n];
}
