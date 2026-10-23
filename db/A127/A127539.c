/*
 * A127539  Number of ordered trees with n edges having no odd-length branches
 *          starting at the root.
 * https://oeis.org/A127539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 5‑term recurrence (-n+1)*a(n) +
 *            2*(n-3)*a(n-1) + (7*n-25)*a(n-2) + (3*n-17)*a(n-3) +
 *            (3*n-7)*a(n-4) + 2*(2*n-9)*a(n-5) = 0 with initial values
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=3
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  computed_max = 4;   /* last index that has been computed */

/* Compute a(n) for n >= 0 */
seq_t a(idx_t n)
{
    /* Base values for n = 0..4 */
    static const seq_t base[5] = { (seq_t)1, (seq_t)0, (seq_t)1,
                                   (seq_t)0, (seq_t)3 };

    if (n < 5)
        return base[n];

    /* Allocate or expand the cache if required */
    if (cache == NULL) {
        cache_size = n + 1;
        if (cache_size < 6)            /* at least indices 0..5 */
            cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 5 && i < cache_size; ++i)
            cache[i] = base[i];
        computed_max = 4;
    } else if (cache_size <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_size = n + 1;
    }

    /* Compute all missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* Accumulate positive and negative contributions separately
           to avoid underflow when using unsigned seq_t. */
        seq_t pos  = 0;
        seq_t neg  = 0;

        /* 2*(i-3)*a(i-1)   (positive for i>=3) */
        pos += (seq_t)(2 * (i - 3)) * cache[i - 1];

        /* (7*i-25)*a(i-2)  (positive for i>=4) */
        pos += (seq_t)(7 * i - 25) * cache[i - 2];

        /* (3*i-17)*a(i-3)  (positive for i>=6, negative for i=5) */
        {
            long long coeff = 3 * (long long)i - 17;
            if (coeff >= 0)
                pos += (seq_t)(coeff) * cache[i - 3];
            else
                neg += (seq_t)(-coeff) * cache[i - 3];
        }

        /* (3*i-7)*a(i-4)   (positive for i>=3) */
        pos += (seq_t)(3 * i - 7) * cache[i - 4];

        /* 2*(2*i-9)*a(i-5) (positive for i>=5) */
        pos += (seq_t)(2 * (2 * i - 9)) * cache[i - 5];

        /* Numerator = pos - neg  (must be non‑negative) */
        seq_t numerator = pos;
        numerator -= neg;

        /* Denominator = i-1  (positive for i>=2) */
        cache[i] = numerator / (seq_t)(i - 1);
    }

    computed_max = n;
    return cache[n];
}
