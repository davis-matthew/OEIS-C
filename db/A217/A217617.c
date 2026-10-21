/*
 * A217617  G.f.: Sum_{n>=0} x^n * Sum_{k=0..n} binomial(n,k)^2 *
 *          x^k*(3-x)^(n-k).
 * https://oeis.org/A217617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * n*a(n) = 3*(2*n-1)*a(n-1) -
 *            9*(n-1)*a(n-2) + 6*(2*n-3)*a(n-3) - 4*(n-2)*a(n-4) * with
 *            initial terms a(0)=1, a(1)=3, a(2)=9, a(3)=33
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;          /* number of valid entries in cache */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cap = (n >= 4 ? (size_t)(n + 1) : 4);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – caller should handle */

        /* base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)33;
        len = 4;
    }

    /* if result already cached */
    if (n < len)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence only valid for i >= 4; for 0..3 cache already set */
        seq_t t1 = (seq_t)3;
        t1 *= (seq_t)(2*i - 1);
        t1 *= cache[i-1];

        seq_t t2 = (seq_t)9;
        t2 *= (seq_t)(i - 1);
        t2 *= cache[i-2];

        seq_t t3 = (seq_t)6;
        t3 *= (seq_t)(2*i - 3);
        t3 *= cache[i-3];

        seq_t t4 = (seq_t)4;
        t4 *= (seq_t)(i - 2);
        t4 *= cache[i-4];

        seq_t tmp = t1 - t2;
        tmp += t3;
        tmp -= t4;

        cache[i] = tmp / (seq_t)i;
    }

    len = (size_t)(n + 1);
    return cache[n];
}
