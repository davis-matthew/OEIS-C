/*
 * A204363  Number of (n+2)X3 0..2 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 3*a(n-3) - 6*a(n-4) + 12*a(n-5) +
 *            14*a(n-6) - 84*a(n-7) + 164*a(n-8) - 292*a(n-9) + 392*a(n-10) -
 *            392*a(n-11) + 640*a(n-12) - 896*a(n-13) + 64*a(n-14) +
 *            2176*a(n-15) - 3584*a(n-16) + 2304*a(n-17) - 512*a(n-18), with
 *            initial values a(1)=74, a(2)=212, a(3)=555, a(4)=891...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[18] = {
    74ULL, 212ULL, 555ULL, 891ULL, 2419ULL, 6121ULL, 13693ULL,
    38041ULL, 99753ULL, 256077ULL, 702265ULL, 1904821ULL,
    5140593ULL, 14093533ULL, 38567033ULL, 105514613ULL,
    289594209ULL, 794670109ULL
};

static const int coeffs[18] = {
    5, -7, 3, -6, 12, 14, -84, 164, -292, 392,
    -392, 640, -896, 64, 2176, -3584, 2304, -512
};

static seq_t *cache = NULL;
static idx_t cached_max = 0;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used by the problem but safe */

    if (n <= 18) {
        return init_vals[n - 1];
    }

    /* Ensure cache has space up to n */
    if (n > cached_max) {
        size_t needed = (size_t)(n + 1); /* index 0 unused */
        if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
            if (!cache) exit(1);
            for (idx_t i = 1; i <= 18; ++i) {
                cache[i] = init_vals[i - 1];
            }
            cache_size = needed;
            cached_max = 18;
        } else if (needed > cache_size) {
            seq_t *p = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!p) exit(1);
            cache = p;
            cache_size = needed;
        }

        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 0; k < 18; ++k) {
                size_t idx = (size_t)(i - (idx_t)(k + 1));
                seq_t term = cache[idx];
                int c = coeffs[k];
                seq_t prod = term * (seq_t)((c >= 0) ? c : -c);
                if (c >= 0)
                    sum_pos += prod;
                else
                    sum_neg += prod;
            }
            cache[i] = sum_pos - sum_neg;
        }
        cached_max = n;
    }

    return cache[n];
}
