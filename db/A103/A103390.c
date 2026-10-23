/*
 * A103390  Natural numbers but with nonprimes squared.
 * https://oeis.org/A103390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n - 1) when (n - 1) is prime; otherwise a(n) = (n - 1)^2
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t m = n - 1;          /* integer whose square or value is returned */
    if (is_prime(m))
        return (seq_t)m;      /* keep primes unchanged */
    seq_t mm = (seq_t)m;
    return mm * mm;          /* square non‑primes (including 0 and 1) */
}
