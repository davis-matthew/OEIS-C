/*
 * A228363  Sorted entries of the multiplication table a*b, with a>1, b>1.
 * https://oeis.org/A228363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sorted multiset { a*b | a>1, b>1 } (OEIS A228363)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each composite number m occurs \tau(m)-2 times, where \tau(m) is the
   number of divisors of m.  Primes and 1 do not appear.  The sequence
   is generated incrementally and memoised so calls may arrive in any
   order. */

#include <stdint.h>
#include <stdlib.h>

/* count the number of divisors of a positive integer n (n <= UINT64_MAX) */
static size_t divisor_count_u64(uint64_t n)
{
    if (n == 0 || n == 1)
        return 1;   /* conventions: tau(1) = 1, tau(0) not used */

    size_t tau = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (m > 1)            /* remaining prime factor */
        tau *= 2;
    return tau;
}

/* memoised cache of sequence values */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 3;   /* integer last examined (≥3) */

/* ensure cache has room for at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n < 1, return 0 as safety */

    /* generate sequence terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        ++last_checked;          /* next integer to test */

        /* skip 1 and 2 (not composite) */
        if (last_checked <= 2)
            continue;

        size_t tau = divisor_count_u64(last_checked);
        if (tau == 2)            /* prime: skip */
            continue;

        size_t count = tau - 2;   /* multiplicity of the composite */

        /* add the value 'count' times */
        for (size_t i = 0; i < count; ++i) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
