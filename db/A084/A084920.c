/*
 * A084920  a(n) = (prime(n)-1)*(prime(n)+1).
 * https://oeis.org/A084920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 - 1 The nth prime is obtained by the helper
 *            get_prime(n). The sequence is thus simply the square of the nth
 *            prime minus one
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the nth prime (1‑based indexing) */
    seq_t p = get_prime(n);
    /* compute p^2 - 1 */
    seq_t res = p * p;
    res -= 1;
    return res;
}
