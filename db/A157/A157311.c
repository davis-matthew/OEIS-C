/*
 * A157311  G.f.: exp( Sum_{n>=1} a(n)*x^n/n ) = Product_{n>=1} (1 +
 *          a(n-1)*x^n).
 * https://oeis.org/A157311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} -d * (-a(d-1))^(n/d) , a(0) = 1 (All arithmetic
 *            is performed with type seq_t. The function uses a lazy, memoised
 *            table, so calls may be in any order.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Lazy memoisation table.  cache[i] holds a(i) for i>0;
   cache[0] is never stored (a(0)=1). */
static seq_t *cache = NULL;
static idx_t  cache_limit = 0;   /* size of allocated cache   */
static idx_t  last_computed = 0; /* highest index that has been computed */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_limit)
        return;                       /* already large enough */

    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failed */
    cache = tmp;

    /* Initialise newly allocated entries to zero */
    for (idx_t i = cache_limit; i <= n; ++i)
        cache[i] = 0;

    cache_limit = n + 1;

    /* Cache[0] must contain the value a(0)=1 the first time we
       allocate the table.  Subsequent allocations leave it
       untouched. */
    if (cache_limit > 0 && cache[0] == 0)
        cache[0] = 1;
}

/* Return a(n) for any n ≥ 0, computing missing terms lazily. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0)=1 */

    ensure_cache(n);

    /* Compute any terms that have not been computed yet */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Enumerate divisors of i up to sqrt(i) */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d)
                continue;   /* not a divisor */

            /* ----- first divisor d ----- */
            {
                idx_t e = i / d;                     /* exponent n/d */
                seq_t pow = big_pow(cache[d - 1], (seq_t)e);
                /* term : d*pow if e odd,   -d*pow if e even */
                if (e & 1ULL)
                    sum += (seq_t)d * pow;
                else
                    sum -= (seq_t)d * pow;
            }

            /* ----- complementary divisor q = i/d, if distinct ----- */
            idx_t q = i / d;
            if (q != d) {
                idx_t e = d;                         /* exponent n/q == d */
                seq_t pow = big_pow(cache[q - 1], (seq_t)e);
                /* term : q*pow if e odd,   -q*pow if e even */
                if (e & 1ULL)
                    sum += (seq_t)q * pow;
                else
                    sum -= (seq_t)q * pow;
            }
        }

        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
