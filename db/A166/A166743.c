/*
 * A166743  a(n) = (2^p - p^2 - 1)/6 where p = prime(n).
 * https://oeis.org/A166743
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^p - p^2 - 1)/6 where p = prime(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute 2^p using arbitrary precision arithmetic. */
    seq_t two_pow = big_pow((seq_t)2, (seq_t)p);

    /* Compute p^2. */
    seq_t p_seq = (seq_t)p;
    seq_t p2 = p_seq * p_seq;

    /* Compute (2^p - p^2 - 1) / 6. */
    seq_t numer = two_pow - p2 - 1;
    seq_t result = numer / 6;

    return result;
}
