/*
 * A076649  Number of digits required to write the prime factors of n.
 * https://oeis.org/A076649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{p^e‖n} e * digits(p)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the total number of decimal digits needed to write
 * the prime factorization of n, counting multiplicities. */

#include <stdint.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t tmp = (uint64_t)n;
    seq_t sum = 0;

    /* Factor n by trial division.  Works efficiently for
     * the sizes required by the test harness. */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;
        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        sum += (seq_t)e * (seq_t)digit_count(p);
    }

    /* If anything remains, it is a prime factor > sqrt(n). */
    if (tmp > 1) {
        sum += (seq_t)digit_count(tmp);
    }

    return sum;
}
