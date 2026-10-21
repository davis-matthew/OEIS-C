/*
 * A214071  Least m>0 such that 2^n-m and n-m are relatively prime.
 * https://oeis.org/A214071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest m>0 such that gcd(2^n - m, n - m) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _pow2_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_pow2_mod.h"

/* Euclidean gcd for idx_t values */
static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Special case for n==1: by definition a(1)=1 */
    if (n == 1) return (seq_t)1;

    /* Search m from 1 upward until gcd(n-m, 2^n - n) == 1 */
    for (idx_t m = 1;; ++m) {
        idx_t k = n - m;   /* k = n - m, must be >0 for m < n */
        if (k == 0) break; /* Should never happen for valid sequence */

        /* Compute (2^n - n) mod k */
        idx_t pow_mod = _pow2_mod(n, k);
        idx_t n_mod   = n % k;
        idx_t rem = (pow_mod + k - n_mod) % k;   /* (2^n - n) mod k */

        if (_gcd_idx(k, rem) == 1)
            return (seq_t)m;
    }

    /* In theory we always return before reaching here */
    return (seq_t)0;
}
