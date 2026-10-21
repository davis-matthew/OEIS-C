/*
 * A235851  Number of (n+1) X (2+1) 0..3 arrays with the upper median equal to
 *          the lower median in every 2 X 2 subblock.
 * https://oeis.org/A235851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) + 63*a(n-2) - 1386*a(n-3) + 1099*a(n-4) +
 *            35227*a(n-5) - 85405*a(n-6) - 221384*a(n-7) + 716768*a(n-8) +
 *            332972*a(n-9) - 1748080*a(n-10) + 329248*a(n-11) +
 *            1044480*a(n-12) - 421888*a(n-13) (for n ≥ 14) with initial
 *            values a(1) … a(13) provided
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeff[13] = {
    15,      63,     -1386,     1099,     35227,
    -85405, -221384, 716768,   332972, -1748080,
    329248, 1044480, -421888
};

static const seq_t init[13] = {
    (seq_t)688,          (seq_t)9600,        (seq_t)121244,
    (seq_t)1629216,      (seq_t)21301696,    (seq_t)283036828,
    (seq_t)3731137160,   (seq_t)49409402632, (seq_t)652743993308,
    (seq_t)8635064547136, (seq_t)114147434529696,
    (seq_t)1509558147820252, (seq_t)19958656976746504
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */

    /* 0‑based cache: cache[i] = a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first 13 terms */
    if (cache_len == 0) {
        cache_cap = 13;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;
    }

    /* extend cache if required */
    size_t target = (size_t)n;           /* number of terms we need */
    if (target > cache_len) {
        if (target > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < target) new_cap = target;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_len; i < target; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= 13; ++k) {
                int c = coeff[k-1];
                seq_t prod = cache[i - k] * (seq_t)c;
                sum += prod;
            }
            cache[i] = sum;
        }
        cache_len = target;
    }

    return cache[n-1];
}
