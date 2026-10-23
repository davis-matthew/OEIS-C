/*
 * A127581  Smallest prime of the form k*2^n - 1, for k >= 2.
 * https://oeis.org/A127581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime of form k*2^n - 1 for k >= 2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Compute 2^n once. */
    seq_t pow2n = pow2(n);

    /* Search k = 2,3,... for the smallest prime of the form k*2^n - 1. */
    for (idx_t k = 2;; ++k) {
        /* Compute candidate = k * 2^n - 1. */
        seq_t cand = (seq_t)k * pow2n;
        cand -= (seq_t)1;
        /* Test primality. */
        if (is_prime_big(cand)) {
            return cand;
        }
    }
}
