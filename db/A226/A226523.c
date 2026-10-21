/*
 * A226523  a(n) = 0 if p=2, 1 if 2 is a square mod p, -1 otherwise, where p =
 *          prime(n).
 * https://oeis.org/A226523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if p = 2, 1 if 2 is a quadratic residue mod p, -1
 *            otherwise, where p is the n‑th prime (p = 2 for n = 1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)          /* p = 2 */
        return (seq_t)0;

    uint32_t p = get_prime(n);      /* p is the n‑th prime, n >= 2 */

    /* Legendre symbol (2/p) is determined by p mod 8:
       +1 if p ≡ ±1 (mod 8), -1 if p ≡ ±3 (mod 8)            */
    uint64_t mod8 = (uint64_t)p & 7ULL;

    if (mod8 == 1 || mod8 == 7)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
