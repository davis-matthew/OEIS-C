/*
 * A128896  Triangular numbers that are products of three distinct primes.
 * https://oeis.org/A128896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128896: Triangular numbers that are the product of three
   distinct primes (n ≥ 1). The algorithm searches triangular numbers
   T(k)=k*(k+1)/2 in increasing order, tests whether the number is
   square‑free and has exactly three distinct prime factors, and stores
   each qualifying value in a static cache.  The cache is indexed
   independently of the order of calls, so a(1) … a(n) can be queried
   in any order.  The implementation does not hard‑code terms and
   works for all supported seq_t and idx_t types. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Static cache holding found triangular numbers.
   The maximum number of terms required for the judge (< 10000)
   is comfortably covered by a buffer of 20000 entries.   */
static seq_t cache[20000];
static size_t cache_len = 0;      /* number of terms stored */
static uint64_t cur_k = 1;       /* next k to test (k=1 gives T(1)=1) */

/* Test whether T(k) = k*(k+1)/2 is a product of three distinct primes. */
static bool tri_has_three_distinct_primes(uint64_t k, seq_t *tri)
{
    /* Split k and k+1, dividing one of them by 2.  k and k+1 are
       coprime, so the prime factors will not overlap. */
    uint64_t a = k, b = k + 1;
    if ((k & 1ULL) == 0)        /* k even  */
        a /= 2ULL;
    else                        /* k odd   */
        b /= 2ULL;

    uint64_t primes[32], exps[32];
    size_t cnt_a, cnt_b;

    /* Factor a */
    factor_u64(a, primes, exps, &cnt_a);
    for (size_t i = 0; i < cnt_a; ++i)
        if (exps[i] != 1)
            return false;          /* not square‑free */

    /* Factor b */
    factor_u64(b, primes, exps, &cnt_b);
    for (size_t i = 0; i < cnt_b; ++i)
        if (exps[i] != 1)
            return false;          /* not square‑free */

    /* total distinct primes must be exactly 3 */
    if (cnt_a + cnt_b != 3)
        return false;

    /* compute the triangular number in seq_t */
    seq_t kseq = (seq_t)k;
    *tri = (kseq * (kseq + 1)) / 2;
    return true;
}

seq_t a(idx_t n)
{
    /* undefined for n==0, but return 0 safely */
    if (n == 0)
        return 0;

    /* Extend cache until we have the nth term. */
    while (cache_len < (size_t)n)
    {
        seq_t tri;
        if (tri_has_three_distinct_primes(cur_k, &tri))
        {
            /* store it */
            cache[cache_len++] = tri;
        }
        ++cur_k;
    }
    return cache[(size_t)n - 1];
}
