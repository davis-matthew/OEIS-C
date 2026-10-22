/*
 * A007921  Numbers that are not the difference of two primes.
 * https://oeis.org/A007921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   odd numbers k such that k+2 is composite (search over odd k; an
 *            odd difference of primes must be (k+2) - 2, and even numbers are
 *            conjecturally always differences of primes)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a007921_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a007921_isprime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    for (;;) {
        if (!a007921_isprime(k + 2)) {
            count++;
            if (count == n) break;
        }
        k += 2;
    }
    return (seq_t)k;
}
