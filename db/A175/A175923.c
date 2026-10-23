/*
 * A175923  Index of the first occurrence of prime(n)-1 consecutive zeros in
 *          the sequence of Bell numbers reduced modulo the n-th prime
 *          prime(n).
 * https://oeis.org/A175923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^p - 1)/(p-1) + 1 - (p^p - p)/(p-1)^2, where p =
 *            prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
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
    /* Convert to the n-th prime */
    seq_t p = (seq_t)get_prime(n);           /* p = prime(n)  */
    /* Compute p^p */
    seq_t pp = big_pow(p, p);                /* p^p                */
    /* Denominator p-1 and its square */
    seq_t denom = p - 1;
    seq_t denom_sq = denom * denom;
    /* First part: (p^p - 1)/(p-1) + 1 */
    seq_t part1 = (pp - 1) / denom;
    seq_t term1 = part1 + (seq_t)1;
    /* Second part: (p^p - p)/(p-1)^2 */
    seq_t term2 = (pp - p) / denom_sq;
    /* Result */
    return term1 - term2;
}
