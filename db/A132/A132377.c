/*
 * A132377  a(n) = PrimePi(n)^n.
 * https://oeis.org/A132377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = PrimePi(n)^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pi = prime_pi(n);          /* number of primes ≤ n */
    return big_pow(pi, (seq_t)n);    /* raise to the n‑th power */
}
