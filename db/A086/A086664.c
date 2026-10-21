/*
 * A086664  a(n) = n - (sum of prime power components of n).
 * https://oeis.org/A086664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Σ p^e (sum over prime power components of n)
 * Range:     u8 n <= 279, u16 n <= 9390, u32 n <= 9390, u64 n <= 9390,
 *            u128 n <= 9390, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert n to seq_t once */
    seq_t result = (seq_t)n;
    /* Work on a 64‑bit copy for factoring */
    uint64_t tmp = n;
    uint64_t sum = 0;

    /* Factor 2 separately for speed */
    if (tmp % 2 == 0) {
        uint64_t power = 1;
        while (tmp % 2 == 0) {
            tmp /= 2;
            power *= 2;
        }
        sum += power;
    }

    /* Test odd divisors up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= p;
            }
            sum += power;
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1) {
        sum += tmp;
    }

    /* Subtract the sum from n */
    result -= (seq_t)sum;
    return result;
}
