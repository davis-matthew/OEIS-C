/*
 * A082490  Exponent of highest power of 3 dividing sum(0<=k<n, C(2n,n)).
 * https://oeis.org/A082490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = 2*v3(n) + (2*digit_sum_base3(n) - digit_sum_base3(2*n))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base3, v3_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v3(x) = exponent of the highest power of 3 dividing x           */
/* digit_sum_base3(x) = sum of the base‑3 digits of x                */

#include <stdint.h>
#include "../utils/digit_sum_base3.h"
#include "../utils/v3_idx.h"

seq_t a(idx_t n) {
    if (n == 0)   /* not used, but define for safety */
        return 0;

    /* exponent of 3 in n */
    seq_t v3n = v3_idx(n);

    /* sum of base‑3 digits of n and of 2*n */
    seq_t s3n   = digit_sum_base3(n);
    seq_t s32n  = digit_sum_base3(n * 2);

    /* exponent of 3 in C(2n,n) = (2*s3n - s32n)/2   */
    seq_t v3binom = (2 * s3n - s32n) >> 1;   /* numerator always even */

    /* final result */
    return 2 * v3n + v3binom;
}
