/*
 * A174744  (2^p-1)^2 where p is prime.
 * https://oeis.org/A174744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^{p_n} – 1)^2 where p_n is the nth prime (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for NULL, size_t */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (1‑indexed). */
    uint64_t p = get_prime(n);

    /* Compute 2^p. */
    seq_t two_p = pow2(p);

    /* Compute (2^p)^2 = 2^{2p}. */
    seq_t res = two_p * two_p;

    /* Subtract 2 * 2^p. */
    res -= two_p * (seq_t)2;

    /* Add 1 to obtain (2^p – 1)^2. */
    res += (seq_t)1;

    return res;
}
