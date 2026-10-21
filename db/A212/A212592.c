/*
 * A212592  a(n) is the difference between multiples of 7 with even and odd
 *          digit sum in base 6 in interval [0,6^n).
 * https://oeis.org/A212592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 21*a(n-2)-35*a(n-4)+7*a(n-6) (n>=7) Initial values:
 *            a(1)=1, a(2)=6, a(3)=11, a(4)=106, a(5)=201, a(6)=2022
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_up_to = 0;   /* greatest n that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the offset 1 sequence */

    /* Allocate/cache initial data on first call */
    if (cache == NULL) {
        size_t init_cap = n < 7 ? 7 : n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = init_cap;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)106;
        cache[5] = (seq_t)201;
        cache[6] = (seq_t)2022;
        computed_up_to = 6;
    }
    /* Grow cache if necessary */
    else if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return the known base values directly */
    if (n <= 6) return cache[n];

    /* Compute any missing terms up to n */
    if (computed_up_to < n) {
        for (size_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t val = 21 * cache[i - 2];
            val -= 35 * cache[i - 4];
            val += 7 * cache[i - 6];
            cache[i] = val;
        }
        computed_up_to = n;
    }
    return cache[n];
}
