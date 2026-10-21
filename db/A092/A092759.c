/*
 * A092759  a(n) = prime(n)^7.
 * https://oeis.org/A092759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^7 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 103, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Compute the n‑th term of A092759: the seventh power of the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1, return 0 for safety */
        return 0;

    seq_t p = get_prime(n);          /* nth prime (1‑based index) */
    seq_t res = 1;
    for (int i = 0; i < 7; ++i)      /* multiply p by itself 7 times */
        res *= p;

    return res;
}
