/*
 * A111089  Largest prime factor of 2n.
 * https://oeis.org/A111089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of 2*n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* for safety, though n>=1 in OEIS */
    if (n == 1) return 2;          /* 2*1 = 2, largest prime factor is 2 */

    uint64_t temp = (uint64_t)n;
    uint64_t largest = 0;

    /* factor out 2 */
    if ((temp & 1ULL) == 0) {
        largest = 2;
        do {
            temp >>= 1;
        } while ((temp & 1ULL) == 0);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            largest = p;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }

    /* if anything remains, it's a prime > sqrt(original) */
    if (temp > 1) {
        largest = temp;
    }

    if (largest < 2) largest = 2;   /* covers the case n=1 handled above */
    return (seq_t)largest;
}
