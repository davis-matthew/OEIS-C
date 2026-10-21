/*
 * A062513  Product of unitary divisors of n is divided by n^(number of
 *          distinct prime factors).
 * https://oeis.org/A062513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^( 2^(k-1) - k ), where k = number of distinct prime
 *            factors of n
 * Range:     u8 n <= 209, u16 n <= 209, u32 n <= 329, u64 n <= 2309,
 *            u128 n <= 3569, big any n
 * Helpers:   distinct_prime_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* for uint64_t */
#include "../utils/distinct_prime_count.h"

/* External function required by the driver */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    int k = distinct_prime_count((uint64_t)n);
    if (k <= 1) return (seq_t)1;   /* exponent zero */

    idx_t exp = ((idx_t)1 << (k - 1)) - k;  /* 2^(k-1) - k */
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)n;

    for (idx_t i = 0; i < exp; ++i)
        result *= base;

    return result;
}
