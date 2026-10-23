/*
 * A023579  Exponent of 2 in prime factorization of prime(n)+3.
 * https://oeis.org/A023579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 in prime(n)+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  prime(n) is accessed via the supplied helper. */
    uint32_t p = get_prime(n);          /* valid for all n ≥ 1 */
    uint64_t tmp = (uint64_t)p + 3ULL;  /* use 64‑bit to avoid overflow in small seq_t */

    seq_t exp = 0;
    while ((tmp & 1ULL) == 0) {         /* count trailing zeros in binary */
        exp++;
        tmp >>= 1;
    }
    return exp;
}
