/*
 * A136251  a(n) = n-th prime reduced modulo the sum of its digits.
 * https://oeis.org/A136251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime(n) mod digit_sum(prime(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime reduced modulo the sum of its digits. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  For n == 0 we return 0 as a safe default. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Get the n-th prime (1‑based index).  get_prime returns a 32‑bit value,
       which is sufficient for n up to at least 100 000. */
    uint64_t prime = (uint64_t)get_prime(n);

    /* Compute the sum of decimal digits of the prime. */
    uint64_t t = prime;
    uint64_t digit_sum = 0;
    while (t > 0) {
        digit_sum += t % 10;
        t /= 10;
    }

    /* Reduce modulo the digit sum. */
    uint64_t remainder = prime % digit_sum;

    /* Return as seq_t.  The value will fit in seq_t for all n in the
       intended range; the cast is safe for the arbitrary‑precision build
       as well. */
    return (seq_t)remainder;
}
