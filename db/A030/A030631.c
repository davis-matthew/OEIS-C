/*
 * A030631  Numbers with 13 divisors.
 * https://oeis.org/A030631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n))^12 A030631: numbers with 13 divisors = 12th
 *            powers of primes
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 257,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Retrieve the n-th prime (n≥1) */
    seq_t p = get_prime(n);
    /* Compute the 12th power */
    return big_pow(p, (seq_t)12);
}
