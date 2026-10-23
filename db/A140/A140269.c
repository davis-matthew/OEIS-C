/*
 * A140269  Numbers n such that gcd((largest divisor of n that is <= sqrt(n)),
 *          (smallest divisor of n that is >=sqrt(n))) > 1.
 * https://oeis.org/A140269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer ≥ 2 such that * let d1 = largest
 *            divisor of n ≤ sqrt(n), * d2 = smallest divisor of n ≥ sqrt(n);
 *            * the sequence consists of those n for which gcd(d1,d2) > 1. *
 *            This is a direct implementation of the definition, computing * n
 *            from the start and caching the results for f...
 * Range:     u8 n <= 50, u16 n <= 6947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* internal gcd for unsigned seq_t */
static inline seq_t _seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* find the largest divisor ≤ sqrt(n)
 * and the corresponding smallest divisor ≥ sqrt(n)
 * (the complementary divisor). */
static inline bool _has_property(uint64_t n)
{
    uint64_t d1 = 1;                          /* largest divisor ≤ sqrt(n)   */
    for (uint64_t i = 2; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (i > d1) d1 = i;               /* keep the largest one found  */
        }
    }
    uint64_t d2 = n / d1;                     /* complementary divisor       */
    return _seq_gcd((seq_t)d1, (seq_t)d2) > (seq_t)1;
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;
    static idx_t   cache_cap  = 0;
    static uint64_t next_val  = 2;            /* next number to examine        */

    /* initialise cache if necessary */
    if (cache_size == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate enough terms */
    while (cache_size < n)
    {
        uint64_t val = next_val++;
        if (_has_property(val))
        {
            if (cache_size + 1 > cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* ignore allocation failure – assuming it succeeds */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)val;
        }
    }

    return cache[n - 1];
}
