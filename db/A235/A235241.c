/*
 * A235241  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 39*a(n-2) - 152*a(n-3) - 441*a(n-4) +
 *            1612*a(n-5) + 1185*a(n-6) - 3128*a(n-7) - 782*a(n-8) for n > 8,
 *            with initial values a(1)=210, a(2)=906, a(3)=3746, a(4)=16150,
 *            a(5)=66874, a(6)=288510, a(7)=1195458, a(8)=5161934
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cache[1 .. cache_size] holds terms */
static idx_t cache_size = 0;       /* highest n that has been computed */

static const seq_t initial[8] = {
    210,   906,   3746,   16150,   66874,   288510,   1195458,  5161934
};

static const int coeff[8] = {
    4, 39, -152, -441, 1612, 1185, -3128, -782
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, safe guard */

    /* Grow cache to include term n if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache = new_cache;

        /* Initialize newly allocated slots up to cache_size (including new items) */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if (i <= 8) {
                cache[i] = initial[i - 1];
            } else {
                /* Compute term using the linear recurrence */
                seq_t pos = 0;
                seq_t neg = 0;
                for (int j = 0; j < 8; ++j) {
                    idx_t idx = i - 1 - j;           /* i-1-j >= 1 for i >= 9 */
                    seq_t term = cache[idx];
                    int c = coeff[j];
                    if (c >= 0) {
                        pos += term * (seq_t)c;
                    } else {
                        neg += term * (seq_t)(-c);
                    }
                }
                cache[i] = pos - neg; /* pos >= neg for all n */
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
