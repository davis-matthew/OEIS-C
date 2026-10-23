/*
 * A166701  Totally multiplicative sequence with a(p) = a(p-1) + 4 for prime
 *          p.
 * https://oeis.org/A166701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e || n} (a(p-1)+4)^e with a(1) = 1. a is totally
 *            multiplicative and for primes p satisfies a(p)=a(p-1)+4
 * Range:     u8 n <= 14, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Cache for previously computed values.  The array is 1‑based; cache[0]
   is unused.  cache[i] is zero iff a(i) has not yet been computed. */
static seq_t *cache = NULL;      /* pointer to cache array        */
static size_t cache_cap = 0;     /* number of elements allocated  */

/* Ensure capacity for indices up to n (inclusive).  If the array is
   already large enough nothing happens. */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;                       /* already enough */

    size_t new_cap = (size_t)n + 1;    /* at least n+1 */
    if (new_cap < 8)
        new_cap = 8;
    else
        new_cap <<= 1;                 /* grow by factor 2 */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                       /* allocation failed – keep the old cache  */

    /* copy existing data */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    /* zero‑initialize the new part */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Integer power of a seq_t base to a non‑negative idx_t exponent.
   The exponent is small for the problem sizes considered. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        result *= base;
        --exp;
    }
    return result;
}

/* Recursive computation of a(n).  Results are cached. */
static seq_t compute_a(idx_t n)
{
    if (n == 1)
        return 1;

    /* cached? */
    if ((size_t)n < cache_cap && cache[n] != 0)
        return cache[n];

    idx_t rem = n;
    seq_t res = 1;

    /* trial division – works fast for n up to at least 1e4. */
    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            idx_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            seq_t sub = compute_a(p - 1);          /* a(p-1)   */
            seq_t base = sub + (seq_t)4;           /* a(p-1)+4 */
            seq_t factor = pow_seq(base, exp);     /* (a(p-1)+4)^exp */
            res *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (rem > 1) {
        idx_t p = rem;
        seq_t sub = compute_a(p - 1);
        seq_t base = sub + (seq_t)4;
        /* exp is 1 here */
        res *= base;
    }

    ensure_cache_size(n);
    cache[n] = res;
    return res;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                       /* out of memory – unlikely in test harness */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
        cache[1] = 1;                       /* a(1) = 1 */
    }

    return compute_a(n);
}
