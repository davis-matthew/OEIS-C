/*
 * A117621  Number of double-perfect partitions of n.
 * https://oeis.org/A117621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(n) = 1 for 2 ≤ n ≤ 5 a(n) = Σ a(m) (2 ≤ m ≤ n-1,
 *            (m−1)|(n−1)) for n ≥ 6 where the sum is taken over all m with
 *            m−1 a divisor of n−1
 * Range:     u8 n <= 240, u16 n <= 8640, u32 n <= 9090, u64 n <= 9090,
 *            u128 n <= 9090, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <cstddef>
#endif

/* Cache for previously computed values.  Accessed by index n. */
static seq_t *cache = NULL;          /* pointer to array of seq_t   */
static idx_t cache_size = 0;         /* total allocated slots       */
static idx_t max_computed = 0;       /* highest n that has been computed */

/* Ensure that cache can store at least indices up to 'n' (inclusive).
   In C++, memory is allocated with 'new' and deleted with 'delete[]'.
   In C, memory is allocated with 'realloc'. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_size)
        return;  /* already enough */

    idx_t new_size = n + 1;
    seq_t *new_cache

#ifdef __cplusplus
    ;
    new_cache = new seq_t[new_size];           /* constructs zero values      */
    /* copy old values */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    delete[] cache;                            /* destroy old array          */
#else
    = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
#endif

    cache = new_cache;
    cache_size = new_size;
}

/* Main function returning the nth term.  It may be called in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1, return 0 */

    /* make sure the cache can hold n */
    ensure_capacity(n);

    /* if already computed, just return it */
    if (n <= max_computed)
        return cache[n];

    /* compute values from (max_computed + 1) up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1)
            cache[k] = (seq_t)0;
        else if (k >= 2 && k <= 5)
            cache[k] = (seq_t)1;
        else {
            idx_t val = k - 1;        /* we need divisors of val */
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= val; ++d) {
                if (val % d == 0) {
                    /* divisor d */
                    if (d < val)          /* m = d+1 < k */
                        sum += cache[d + 1];
                    /* complementary divisor val/d */
                    idx_t d2 = val / d;
                    if (d2 != d && d2 < val)   /* avoid double count and exclude m = k */
                        sum += cache[d2 + 1];
                }
            }
            cache[k] = sum;
        }
    }

    max_computed = n;
    return cache[n];
}
