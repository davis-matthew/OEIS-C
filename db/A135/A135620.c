/*
 * A135620  a(n) = 2^(prime(n) - 2).
 * https://oeis.org/A135620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(prime(n)-2) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* get_prime is 1‑based: get_prime(k) returns the k-th prime. */
    uint32_t p = get_prime(n);
    /* exponent is prime(n)-2, guaranteed ≥0 for n ≥ 1 */
    seq_t exp = (seq_t)(p - 2);
    return big_pow((seq_t)2, exp);
}
