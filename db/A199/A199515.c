/*
 * A199515  Denominators of: MoebiusMu(n)^2*(n/(n - EulerPhi(n))).
 * https://oeis.org/A199515
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of MöbiusMu(n)^2 * (n/(n - EulerPhi(n))) = {
 *            1, if n is not squarefree { (n - φ(n)) / gcd(n, n - φ(n)),
 *            otherwise. The function is computed directly using trial
 *            division and a local GCD routine. Complexity O(√n) per call,
 *            adequate for n up to ~10⁴
 * Range:     u8 n <= 689, u16 n <= 9834, u32 n <= 9834, u64 n <= 9834,
 *            u128 n <= 9834, big any n
 * Helpers:   _gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gcd_idx_t.h"

/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* We only need values for n >= 2; behavior for smaller n is irrelevant. */
    if (n < 2)
        return (seq_t)1;   /* placeholder, not used */

    idx_t m = n;
    /* Check squarefree: if any prime factor appears more than once, return 1. */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return (seq_t)1;   /* not squarefree */
            }
        }
    }
    if (tmp > 1) {
        /* Remaining prime factor with exponent 1, ok */
    }

    /* Compute Euler's totient φ(n) using its multiplicative property */
    idx_t phi = n;
    tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            phi = phi / p * (p - 1);
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        phi = phi / tmp * (tmp - 1);
    }

    /* Denominator of n/(n-φ) simplified by gcd */
    idx_t d = n - phi;                     /* numerator of reduced fraction */
    idx_t g = _gcd_idx_t(n, d);
    idx_t denom = d / g;
    return (seq_t)denom;
}
