/*
 * A109958  Concatenate n and the sum of primes dividing n (counting
 *          multiplicity).
 * https://oeis.org/A109958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(n, sum_of_prime_factors_with_multiplicity(n))
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Compute the sum of prime factors of n, counting multiplicities. */
    idx_t tmp = n;
    uint64_t sum = 0;

    /* handle factor 2 */
    while ((tmp & 1) == 0) {  /* tmp % 2 == 0 */
        sum += 2;
        tmp >>= 1;           /* tmp /= 2 */
    }

    /* odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        while (tmp % p == 0) {
            sum += p;
            tmp /= p;
        }
    }

    /* if remainder is a prime > 1 */
    if (tmp > 1) {
        sum += tmp;
    }

    /* Determine the number of decimal digits of sum.
       For sum == 0 we treat it as one digit ("0"). */
    idx_t digits = 1;
    if (sum != 0) {
        uint64_t t = sum;
        digits = 0;
        while (t > 0) {
            ++digits;
            t /= 10;
        }
    }

    /* Compute 10^digits as seq_t. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < digits; ++i) {
        pow10 *= 10;
    }

    /* Concatenate n and sum: result = n * 10^digits + sum. */
    seq_t result = ((seq_t)n) * pow10 + (seq_t)sum;
    return result;
}
