/*
 * A214824  Number of solid standard Young tableaux of shape [[(2)^n],[2]].
 * https://oeis.org/A214824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) =
 *            (2*(2*n+1)*(n^2+5*n+2)*n)/((n-1)*(n+3)*(n^2+3*n-2)) * a(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* External linkage as required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)               /* outside OEIS range */
        return 0;

    /* Cache to allow arbitrary call order */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of allocated slots (max index + 1) */

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();             /* out-of-memory abort */
        cache = tmp;

        /* Fill new cache entries, starting with the smallest not yet computed */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) continue;      /* unused slot, keep zero */
            if (i == 1) {
                cache[1] = (seq_t)2;
                continue;
            }

            seq_t result = cache[i-1];   /* a(i-1) */

            /* Helper: cast i to seq_t once */
            seq_t ii = (seq_t)i;

            /* Numerators: 2, (2i+1), (i^2+5i+2), i */
            seq_t num_arr[4];
            num_arr[0] = (seq_t)2;
            num_arr[1] = (seq_t)2 * ii + 1;
            num_arr[2] = ii*ii + (seq_t)5 * ii + 2;
            num_arr[3] = ii;

            /* Denominators: (i-1), (i+3), (i^2+3i-2) */
            seq_t den_arr[3];
            den_arr[0] = ii - 1;
            den_arr[1] = ii + 3;
            den_arr[2] = ii*ii + (seq_t)3 * ii - 2;

            /* Cancel factors between current result and denominators */
            for (int d = 0; d < 3; ++d) {
                seq_t g = gcd(result, den_arr[d]);
                result /= g;
                den_arr[d] /= g;
            }

            /* Cancel remaining denominator factors with numerators */
            for (int d = 0; d < 3; ++d) {
                for (int m = 0; m < 4 && den_arr[d] > 1; ++m) {
                    seq_t g = gcd(num_arr[m], den_arr[d]);
                    num_arr[m] /= g;
                    den_arr[d] /= g;
                }
            }

            /* Multiply the remaining numerators into the result */
            for (int m = 0; m < 4; ++m)
                result = result * num_arr[m];

            /* Any remaining denominator factors (should be 1) */
            for (int d = 0; d < 3; ++d)
                if (den_arr[d] > 1)
                    result = result / den_arr[d];

            cache[i] = result;
        }

        cache_len = new_len;
    }

    return cache[n];
}
