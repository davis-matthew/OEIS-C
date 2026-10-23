/*
 * A023572  Exponent of 2 in prime factorization of prime(n) - 3.
 * https://oeis.org/A023572
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 in (prime(n)-3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) {
        /* undefined for indices below the offset */
        return (seq_t)0;
    }

    /* obtain the n-th prime (1‑indexed: p(1)=2) */
    uint32_t p32 = get_prime(n);
    seq_t diff = (seq_t)p32 - (seq_t)3;
    seq_t exp = (seq_t)0;

    /* count how many times diff is divisible by 2 */
    while ((diff & (seq_t)1) == (seq_t)0) {
        exp++;
        diff >>= 1;
    }

    return exp;
}
