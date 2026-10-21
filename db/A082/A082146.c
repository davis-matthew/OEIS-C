/*
 * A082146  Expansion of g.f.: (1+x^5)/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^6)).
 * https://oeis.org/A082146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-5) + a(n-6) - 2*a(n-7) + a(n-8) -
 *            a(n-9) + a(n-11) + a(n-13) - a(n-14); Base values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=4, a(7)=3, a(8)=6,
 *            a(9)=6, a(10)=8, a(11)=9, a(12)=13, a(13)=12, a(14)=17
 * Range:     u8 n <= 43, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[] = {
    (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
    (seq_t)4, (seq_t)3, (seq_t)6, (seq_t)6, (seq_t)8, (seq_t)9,
    (seq_t)13, (seq_t)12, (seq_t)17
};
static const size_t base_count = sizeof(base_vals)/sizeof(base_vals[0]);

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    // handle base cases directly
    if (n < base_count)
        return base_vals[(size_t)n];

    // initialize cache with base values if not done yet
    if (cache_size == 0) {
        cache_size = base_count;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = base_vals[i];
    }

    // extend cache up to n
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val = cache[i-1]
                        + cache[i-3]
                        - cache[i-5]
                        + cache[i-6]
                        - (seq_t)2 * cache[i-7]
                        + cache[i-8]
                        - cache[i-9]
                        + cache[i-11]
                        + cache[i-13]
                        - cache[i-14];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
