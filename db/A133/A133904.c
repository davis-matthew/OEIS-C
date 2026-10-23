/*
 * A133904  a(n) = gcd(a(n-1),n-1)*a(n-1) + d(n-1) if a(n-1) is not divisible
 *          by 2, otherwise a(n) = a(n-1)/2, where gcd denotes common divisor,
 *          d(n) is number of divisors of n.
 * https://oeis.org/A133904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: if a(n‑1) is even: a(n) = a(n‑1)/2 else:
 *            a(n) = gcd(a(n‑1), n‑1) * a(n‑1) + d(n‑1) where d(k) = number of
 *            positive divisors of k
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 93, u64 n <= 158,
 *            u128 n <= 282, big any n
 * Helpers:   _divisor_count_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/_divisor_count_uint64.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t sz = 0;         /* number of terms currently stored */
    static size_t cap = 0;        /* allocated size of cache */

    if (n < 1)     /* offset is 1; behaviour for n<1 is undefined */
        return (seq_t)0;

    /* Ensure the cache contains terms up to n. */
    if ((size_t)n > sz)
    {
        size_t need = n;          /* we must compute indices 1..n */

        /* Allocate or grow the cache as needed. */
        if (cap == 0)
        {
            cap = 4;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        while (cap < need)
        {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* Compute missing terms. */
        for (size_t i = sz; i < need; ++i)
        {
            idx_t idx = i + 1;     /* 1‑based index for term a(idx) */

            if (i == 0)
            {
                cache[i] = (seq_t)1;   /* a(1) = 1 */
                continue;
            }

            seq_t prev = cache[i - 1];

            if ((prev & (seq_t)1) == 0)    /* prev is even */
            {
                cache[i] = prev / 2;
            }
            else
            {
                seq_t g   = gcd(prev, (seq_t)(idx - 1));
                seq_t prod = g * prev;
                seq_t dcnt = _divisor_count_uint64(idx - 1);
                cache[i] = prod + dcnt;
            }
        }
        sz = need;
    }

    return cache[n - 1];   /* cache is 0‑based, n is 1‑based */
}
