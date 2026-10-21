/*
 * A213698  Half the number of 3 X 3 0..n symmetric arrays with no 2 X 2
 *          subblock summing to 2n.
 * https://oeis.org/A213698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 11*a(n-3) + 6*a(n-4) + 14*a(n-5) -
 *            14*a(n-6) - 6*a(n-7) + 11*a(n-8) - a(n-9) - 3*a(n-10) + a(n-11),
 *            with initial values a(1)=13, a(2)=192, a(3)=1320, a(4)=5470,
 *            a(5)=17499, a(6)=45892, a(7)=105856, a(8)=219564, a(9)=421825,
 *            a(10)=758560, a(11)=1296408. The sequence is d...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 44, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static const seq_t init_values[11] = {
    13, 192, 1320, 5470, 17499, 45892, 105856, 219564, 421825, 758560, 1296408
};

static const int coeffs[11] = {3, 1, -11, 6, 14, -14, -6, 11, -1, -3, 1};

static seq_t *cache = NULL;   /* cache of computed terms */
static size_t cache_size = 0; /* number of terms currently stored */

/* grow the cache to at least 'new_cap' capacity */
static void grow_cache(size_t new_cap) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else if (new_cap > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
    }
    /* no need to initialize new slots; they will be filled in a() */
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n = 0, but return 0 */

    size_t wanted = (size_t)n;

    if (cache_size < wanted) {
        size_t new_cap = wanted > 2 * cache_size ? wanted : 2 * cache_size;
        if (new_cap < wanted) new_cap = wanted;
        grow_cache(new_cap);

        /* fill missing entries */
        for (size_t i = cache_size; i < wanted; ++i) {
            if (i < 11) {
                cache[i] = init_values[i];
            } else {
                seq_t sum = 0;
                for (int k = 1; k <= 11; ++k) {
                    if (coeffs[k-1] >= 0)
                        sum += (seq_t)coeffs[k-1] * cache[i - k];
                    else
                        sum -= (seq_t)(-coeffs[k-1]) * cache[i - k];
                }
                cache[i] = sum;
            }
        }
        cache_size = wanted;
    }

    return cache[(size_t)n - 1];
}
