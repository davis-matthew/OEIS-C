/*
 * A138031  a(n) = prime(n)^13.
 * https://oeis.org/A138031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^13 a(1) = 2^13, a(2) = 3^13, a(3) = 5^13,
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 157,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined for the OEIS offset 1; return 0 in that case. */
    if (n == 0) return (seq_t)0;

    /* get_prime(k) returns the k‑th prime number where k is 1‑based. */
    seq_t p = (seq_t)get_prime(n);      /* nth prime */
    return seq_pow(p, 13);              /* compute p^13 exactly */
}
