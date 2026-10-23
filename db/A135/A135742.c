/*
 * A135742  E.g.f.: A(x) = Sum_{n>=0} exp( n*(n-1)/2 * x ) * x^n / n!.
 * https://oeis.org/A135742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * ( k*(k-1)/2 )^(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n >= cap) {
        size_t newcap = n * 2 + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = 0;          /* 0 is an unused sentinel value */
        cap = newcap;
    }

    if (cache[n] != 0)
        return cache[n];

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binom(n, k);          /* C(n,k) */

        /* Skip terms that are guaranteed to be zero. */
        if (k < 2 && n != k)
            continue;

        seq_t term = 0;

        if (k < 2) {                     /* k=0 or k=1 → p_k = 0 */
            term = bin;                   /* power = 1 when n==k */
        } else {
            seq_t pk = (seq_t)k * (k - 1) / 2;          /* p_k */
            seq_t pow = big_pow(pk, (seq_t)(n - k));    /* p_k^(n-k) */
            term = bin * pow;                            /* binomial * power */
        }

        sum += term;
    }

    cache[n] = sum;
    return sum;
}
