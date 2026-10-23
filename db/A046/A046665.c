/*
 * A046665  Largest prime divisor of n - smallest prime divisor of n (a(1)=0).
 * https://oeis.org/A046665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A006530(n) - A020639(n) (largest prime factor minus
 *            smallest prime factor)
 * Range:     u8 n <= 525, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"   /* uint64_t _smallest_prime_factor(uint64_t n); */
#include "../utils/largest_prime_factor.h"     /* uint64_t largest_prime_factor(uint64_t n); */

seq_t a(idx_t n) {
    /* n is unsigned 64‑bit; handle n == 1 separately */
    if (n < 2) return (seq_t)0;

    /* Compute the largest and smallest prime factors using helper routines */
    uint64_t n64 = (uint64_t)n;                    /* safe conversion because n <= UINT64_MAX */
    uint64_t lpf = largest_prime_factor(n64);      /* largest prime factor of n */
    uint64_t spf = _smallest_prime_factor(n64);    /* smallest prime factor of n */

    /* difference fits in seq_t because lpf >= spf and both <= n */
    return (seq_t)(lpf - spf);
}
