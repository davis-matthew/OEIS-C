/*
 * A064505  Interchange primes and their squares.
 * https://oeis.org/A064505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is neither prime nor a square of a prime; ** if n
 *            is prime, a(n) = n^2; ** if n = p^2 with p prime, a(n) = p. **
 *            This defines a self‑inverse permutation of the natural numbers
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floorSqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/floorSqrt.h"

/* simple deterministic 64‑bit isPrime test (trial division up to sqrt) */
static inline int isPrimeUint64(uint64_t n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, but guard for 0 */
    uint64_t un = (uint64_t)n;

    /* check if n is a prime */
    if (isPrimeUint64(un)) {
        seq_t nn = (seq_t)un;
        return nn * nn;                  /* n^2 */
    }

    /* check if n is a square of a prime */
    uint64_t r = floorSqrt(un);
    if (r * r == un && isPrimeUint64(r)) {
        return (seq_t)r;
    }

    /* otherwise fixed point */
    return (seq_t)un;
}
