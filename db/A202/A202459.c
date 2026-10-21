/*
 * A202459  Number of (n+2) X 8 binary arrays with consecutive windows of
 *          three bits considered as a binary number nondecreasing in every
 *          row and column.
 * https://oeis.org/A202459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) * +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * (for n
 *            > 9). Initial values: * a(1)=2197, a(2)=5854, a(3)=14586,
 *            a(4)=33468, a(5)=71088, * a(6)=141192, a(7)=264822, a(8)=473031,
 *            a(9)=810265
 * Range:     u8 none, u16 n <= 4, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[9] = {
    2197ULL, 5854ULL, 14586ULL, 33468ULL, 71088ULL,
    141192ULL, 264822ULL, 473031ULL, 810265ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) /* undefined, but guard against accidental use */
        return 0;

    /* Return cached value if already known */
    if (cache && n <= cache_size)
        return cache[n - 1];

    /* Initialise cache with the first nine terms */
    if (!cache) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* Extend cache up to n and compute missing values */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) /* allocation failed, return 0 to indicate error */
            return 0;
        cache = tmp;
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* k is the 1‑based index of the term to compute */
            seq_t term = 0;
            term += cache[k - 2] * (seq_t)9;     /* a(k-1) */
            term -= cache[k - 3] * (seq_t)36;    /* a(k-2) */
            term += cache[k - 4] * (seq_t)84;    /* a(k-3) */
            term -= cache[k - 5] * (seq_t)126;   /* a(k-4) */
            term += cache[k - 6] * (seq_t)126;   /* a(k-5) */
            term -= cache[k - 7] * (seq_t)84;    /* a(k-6) */
            term += cache[k - 8] * (seq_t)36;    /* a(k-7) */
            term -= cache[k - 9] * (seq_t)9;     /* a(k-8) */
            term += cache[k -10];                /* a(k-9) */
            cache[k - 1] = term;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
