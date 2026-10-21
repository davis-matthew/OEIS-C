/*
 * A083910  Number of divisors of n that are congruent to 0 modulo 10.
 * https://oeis.org/A083910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are multiples of 10. A
 *            divisor d of n is a multiple of 10 iff d contains at least one
 *            factor 2 and at least one factor 5. Let n = 2^e2 · 5^e5 · m
 *            where gcd(m,10)=1. Then the divisors of n that are multiples of
 *            10 are 2^i · 5^j · d', 1 ≤ i ≤ e2, 1 ≤ j ≤ e5, d' |...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tau_u64.h"

seq_t a(idx_t n) {
    uint64_t m = n;
    uint8_t e2 = 0, e5 = 0;

    /* count factors of 2 */
    while ((m & 1ULL) == 0) {   /* faster than modulo for powers of 2 */
        ++e2;
        m >>= 1;
    }

    /* count factors of 5 */
    while (m % 5ULL == 0) {
        ++e5;
        m /= 5ULL;
    }

    if (e2 == 0 || e5 == 0)
        return (seq_t)0;

    seq_t tau_m = tau_u64(m);
    return (seq_t)e2 * (seq_t)e5 * tau_m;
}
