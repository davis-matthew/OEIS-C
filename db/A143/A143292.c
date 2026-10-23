/*
 * A143292  Gray code of prime(n) (decimal representation).
 * https://oeis.org/A143292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) ^ (prime(n) >> 1) /* Gray code of the nth prime
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n‑th prime (n ≥ 1) */
    uint32_t p = get_prime(n);

    /* convert to seq_t and compute Gray code */
    seq_t sp = (seq_t)p;
    return sp ^ (sp >> 1u);
}
