/*
 * A215412  McKay-Thompson series of class 18C for the Monster group with a(0)
 *          = -2.
 * https://oeis.org/A215412
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^(n) in (1/x) * G(x) = \prod_{k>=1}
 *            (1 - x^k)^{-c(k)} where c(k) has period 18 c = [-2, 2, 2, 2, -2,
 *            -2, -2, 2, 0, 2, -2, -2, -2, 2, 2, 2, -2, 0]
 * Range:     i8 n <= 15, i16 n <= 48, i32 n <= 160, i64 n <= 562,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers provided by the build – use only their signatures */
#define seq_alloc_new_array(size) ((seq_t *)calloc((size_t)(size), sizeof(seq_t)))

seq_t a(idx_t n) {
    /* Periodic exponents */
    static const int period[18] = {
        -2, 2, 2, 2, -2, -2, -2, 2, 0,
        2, -2, -2, -2, 2, 2, 2, -2, 0
    };

    /* Cache of previously computed coefficients */
    static seq_t *cache = NULL;        /* Holds G(x) coefficients up to cache_max */
    static idx_t cache_size = 0;       /* Size of allocated cache (number of indices) */
    static idx_t cache_max = -1;       /* Highest target index computed (i.e., a(cache_max) == cache[cache_max]) */

    /* Target index in G(x) is n+1 (since a(n) = coefficient of x^{n} = G(x) coefficient of x^{n+1}) */
    idx_t target = n + 1;

    /* If already computed, return value */
    if (cache && target <= cache_max) {
        return cache[target];
    }

    /* Need to recompute up to new target */
    idx_t newsize = target + 1;             /* array will hold indices 0..target */
    seq_t *new_arr = seq_alloc_new_array(newsize);
    seq_t *tmp_arr = seq_alloc_new_array(newsize);

    if (!new_arr || !tmp_arr) { /* allocation failure – return 0 (undefined) */
        free(new_arr);
        free(tmp_arr);
        return 0;
    }

    /* Initialize coefficient 0 to 1, all others are 0 (calloc already did this) */
    new_arr[0] = 1;

    /* Iterate over factors k = 1 .. target */
    for (idx_t k = 1; k <= (idx_t)target; ++k) {
        int c = period[(k - 1) % 18];
        if (c == 0) {
            /* No factor – copy coefficients unchanged */
            for (idx_t i = 0; i < newsize; ++i)
                tmp_arr[i] = new_arr[i];
        } else if (c == -2) {
            /* Factor (1 - x^k)^2 : 1 - 2x^k + x^{2k} */
            for (idx_t i = 0; i < newsize; ++i) {
                seq_t res = new_arr[i];
                if (i >= k) res -= (seq_t)2 * new_arr[i - k];
                if (i >= 2 * k) res += new_arr[i - 2 * k];
                tmp_arr[i] = res;
            }
        } else { /* c == 2 : factor (1 - x^k)^-2 = sum_{t>=0} (t+1) x^{k t} */
            for (idx_t r = 0; r < k; ++r) {
                idx_t idx = r;
                idx_t j = 0;
                seq_t P = 0;              /* cumulative sum of values */
                seq_t Q = 0;              /* cumulative sum of j*value */
                while (idx < newsize) {
                    seq_t val = new_arr[idx];
                    P += val;
                    Q += ((seq_t)j) * val;
                    tmp_arr[idx] = ((seq_t)(j + 1)) * P - Q;
                    idx += k;
                    ++j;
                }
            }
        }
        /* Swap buffers for next iteration */
        seq_t *buf = new_arr;
        new_arr = tmp_arr;
        tmp_arr = buf;
    }

    /* Store new coefficients in cache */
    seq_t *old_cache = cache;
    cache = new_arr;
    cache_size = newsize;
    cache_max = target;

    /* Free previous cache and temporary buffer */
    free(old_cache);
    free(tmp_arr);

    return cache[target];
}
