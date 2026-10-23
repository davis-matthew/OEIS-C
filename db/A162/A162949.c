/*
 * A162949  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * a(0)=1, a(1)=8, a(2)=56,
 *            a(3)=392 * For n=4: a(4)=6*a(3)+6*a(2)+6*a(1)-21*a(0)+1 * For
 *            n>4: a(n)=6*a(n-1)+6*a(n-2)+6*a(n-3)-21*a(n-4) (no constant
 *            term) * * The sequence has generating function *
 *            (t^4+2t^3+2t^2+2t+1)/(1-6t-6t^2-6t^3+21t^4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;

    /* initial base values (n=0..3) */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)392;
        len = 4;
    }

    /* return cached value if already computed */
    if ((size_t)n < len)
        return cache[n];

    /* ensure enough space for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap * 2;
        while ((size_t)(n + 1) > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* compute new values up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t val = cache[idx - 1] * (seq_t)6
                  + cache[idx - 2] * (seq_t)6
                  + cache[idx - 3] * (seq_t)6
                  - cache[idx - 4] * (seq_t)21;
        if (i == (idx_t)4)   /* add the numerator term t^4 */
            val += (seq_t)1;
        cache[idx] = val;
    }
    len = (size_t)(n + 1);

    return cache[n];
}
