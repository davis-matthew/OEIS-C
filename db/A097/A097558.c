/*
 * A097558  Sum{k=1 to oo} a(k)/k^r = sqrt(zeta(r) -3/4) +1/2.
 * https://oeis.org/A097558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n >= 2, a(n) = 1 - Σ_{d|n, 2≤d≤n-1} a(n/d) * a(d)
 * Range:     i8 n <= 143, i16 n <= 2879, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, sum_div_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sum_div_rec.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;          /* number of computed terms */

    if (n == 0) return (seq_t)0;         /* undefined for n < 1 */

    /* Ensure cache has room for up to n terms */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failed */

        cache = new_cache;

        /* Initialise the first term if needed */
        if (cache_cap == 0) {
            cache[0] = (seq_t)1;
            cache_cap = 1;
        }

        /* Compute terms from cache_cap+1 up to n */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            uint64_t num = (uint64_t)i;
            seq_t sum = (seq_t)0;

            /* Factorise num */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(num, primes, exps, &cnt);

            /* Enumerate all proper divisors and accumulate the sum */
            sum_div_rec(primes, exps, cnt, 0, 1, num, &sum, cache);

            /* Apply the recurrence: a(n) = 1 - sum */
            cache[(size_t)(i - 1)] = (seq_t)1 - sum;
        }
        cache_cap = n;
    }
    return cache[(size_t)(n - 1)];
}
