/*
 * A062481  a(n) = n^prime(n).
 * https://oeis.org/A062481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1^2, a(2) = 2^3, … */
    if (n == 0) return 0;          /* undefined for n < 1, safe fallback */

    seq_t prime = get_prime(n);     /* the n‑th prime (2,3,5,…)   */
    return big_pow((seq_t)n, prime);/* n raised to that prime      */
}
