/*
 * A006254  Numbers k such that 2k-1 is prime.
 * https://oeis.org/A006254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A000040(n+1) + 1)/2, i.e. (prime(n+1)+1)/2; found by
 *            searching odd numbers for primes
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a006254_is_odd_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a006254_is_odd_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 1;

    while (count < n) {
        candidate += 2;
        if (a006254_is_odd_prime(candidate)) {
            count++;
        }
    }

    uint64_t k = (candidate + 1) / 2;
    return (seq_t)k;
}
