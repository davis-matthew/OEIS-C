/*
 * A072107  a(n) = Sum_{k=1..n} A014963(k).
 * https://oeis.org/A072107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} A014963(k)
 * Range:     u8 n <= 40, u16 n <= 918, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

/* cache[0] unused, cache[i] = a(i) for i>=1 */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* highest index computed */
static idx_t cache_cap = 0;      /* allocated capacity */

/* A014963(k):
 *  1 if k has more than one distinct prime factor,
 *  otherwise the prime factor (k if k is prime). */
static seq_t spf_value(idx_t k)
{
    if (k == 1)
        return (seq_t)1;

    uint64_t spf_u = _smallest_prime_factor((uint64_t)k);
    if (spf_u == 0)
        return (seq_t)1;

    idx_t spf = (idx_t)spf_u;
    idx_t t = k;
    while (t % spf == 0)
        t /= spf;
    if (t == 1)               /* k is a power of spf (or prime itself) */
        return (seq_t)spf;
    else                      /* k has at least two distinct prime factors */
        return (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* defined only for n>=1 */

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!p)
            exit(1);           /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }

    /* already computed? */
    if (n <= cache_len)
        return cache[n];

    /* start from last computed sum */
    seq_t sum = cache_len ? cache[cache_len] : (seq_t)0;

    /* compute missing terms */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        sum += spf_value(k);
        cache[k] = sum;
    }

    cache_len = n;
    return cache[n];
}
