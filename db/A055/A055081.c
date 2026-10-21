/*
 * A055081  Number of positive integers whose harmonic mean with n is a
 *          positive integer.
 * https://oeis.org/A055081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integers x such that the harmonic mean
 *            of n and x is an integer. Equivalently, a(n) equals the number
 *            of divisors of 2 n² that are greater than n
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enumerate_divisors_gt_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/enumerate_divisors_gt_n.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    /* Factorize n */
    uint64_t tmp = n;
    uint64_t primes[64];
    uint32_t exp_n[64];
    size_t plen = 0;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[plen] = p;
            exp_n[plen] = e;
            ++plen;
        }
    }
    if (tmp > 1) {
        primes[plen] = tmp;
        exp_n[plen] = 1;
        ++plen;
    }

    /* Ensure 2 is present, because 2n² always contains a factor 2¹
       (if n is odd) or 2^(2e₂+1) (if n is even). */
    bool has2 = false;
    for (size_t i = 0; i < plen; ++i)
        if (primes[i] == 2) {
            has2 = true;
            break;
        }
    if (!has2) {
        primes[plen] = 2;
        exp_n[plen] = 0;
        ++plen;
    }

    /* Compute exponents for 2n² */
    uint32_t exp2[64];
    for (size_t i = 0; i < plen; ++i) {
        exp2[i] = exp_n[i] * 2;
        if (primes[i] == 2)
            exp2[i] += 1;
    }

    seq_t count = 0;
    enumerate_divisors_gt_n(primes, exp2, 0, plen, 1, n, &count);
    return count;
}
