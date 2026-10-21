/*
 * A081758  Sum of prime factors (with repetition) of sum of prime factors
 *          (with repetition) of n.
 * https://oeis.org/A081758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001414(A001414(n))
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum of prime factors (with repetition) of the sum of prime factors (with repetition) of n */

/* This implementation relies on the helper function
 * void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt);
 * which decomposes a 64‑bit integer into its prime factors and exponents.
 * Since the input index n is of type idx_t (uint64_t), all factorisations
 * fit inside 64 bits.  The intermediate sums are also bounded by the
 * original number, so uint64_t suffices for the calculations.  The result
 * is converted back to seq_t for the final return value. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Compute the sum of prime factors with repetition of a 64‑bit integer x.
 * The function returns the sum as a seq_t value. */
static seq_t sum_prime_factors_uint64(uint64_t x)
{
    if (x < 2) {
        /* For 0 and 1 the sum of prime factors is defined as the number itself. */
        return (seq_t)x;
    }

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t count = 0;

    /* factor_u64 expects the caller to provide storage for primes and exponents. */
    factor_u64(x, primes, exponents, &count);

    uint64_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        /* primes[i] and exponents[i] are guaranteed to be within 64 bits. */
        sum += primes[i] * exponents[i];
    }

    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Sequence is defined starting at n = 2. Values below the offset are not
           required for correctness, but we return 0 for completeness. */
        return 0;
    }

    /* First application of the sum-of-prime-factors function. */
    seq_t s1 = sum_prime_factors_uint64((uint64_t)n);

    /* Second application. */
    seq_t s2 = sum_prime_factors_uint64((uint64_t)s1);

    return s2;
}
