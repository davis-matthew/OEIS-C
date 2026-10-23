/*
 * A130194  Let M = lower triangular matrix with 1's on and below the main
 *          diagonal, with columns multiplied by +1, +1, -1, -1, repeated;
 *          form M^2; read across rows of resulting triangle.
 * https://oeis.org/A130194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = convolution of the periodic sequence 1,1,-1,-1 with
 *            itself, read across rows of the resulting lower‑triangular
 *            Toeplitz matrix. For a(n), let n-1 = t. Find i such that
 *            i(i+1)/2 ≤ t < (i+1)(i+2)/2. Then j = t - i(i+1)/2 and r = i-j.
 *            The value is a(n) = Σ_{k=0}^{r} s_k · s_{r-k}, where s_k =...
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   conv_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/conv_val.h"

static inline unsigned long long u128_sqrt(unsigned __int128 v) {
    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)1 << 34; /* > sqrt(2^67) */
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* unused; sequence is offset 1 */
    idx_t t = n - 1;
    unsigned __int128 D = 1 + (unsigned __int128)8 * t;
    unsigned long long sqrtD = u128_sqrt(D);
    idx_t i = (idx_t)((sqrtD - 1) / 2);

    /* Adjust i so that i*(i+1)/2 <= t < (i+1)*(i+2)/2. */
    unsigned __int128 tri = (unsigned __int128)i * (i + 1) / 2;
    while (tri > t) {
        --i;
        tri = (unsigned __int128)i * (i + 1) / 2;
    }
    while ((unsigned __int128)(i + 1) * (i + 2) / 2 <= t) {
        ++i;
        tri = (unsigned __int128)i * (i + 1) / 2;
    }

    idx_t j = t - (idx_t)tri;
    idx_t r = i - j;  /* difference determines convolution index */
    return conv_val(r);
}
