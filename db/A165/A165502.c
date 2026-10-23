/*
 * A165502  Primes p where the digital sum of p^2 is equal to 31.
 * https://oeis.org/A165502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 531, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code finds the n-th prime p such that the digital sum of p^2 is 31. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t p = 2;

    while (true) {
        /* Square the prime (fits in 64-bit for all needed p). */
        uint64_t sq = p * p;

        /* Compute the decimal digit sum of the square. */
        uint64_t temp = sq;
        uint64_t sum = 0;
        while (temp > 0) {
            sum += temp % 10;
            temp /= 10;
        }

        if (sum == 31) {
            ++count;
            if (count == n) {
                return (seq_t)p;  /* cast to the return type */
            }
        }

        /* Advance to the next prime. */
        p = next_prime(p);
    }
}
