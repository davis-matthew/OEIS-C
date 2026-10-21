/*
 * A208506  p^(p+1) + (p+1)^p, where p = prime(n).
 * https://oeis.org/A208506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p^(p+1) + (p+1)^p, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Obtain the nth prime.  The helper returns a 32‑bit value, safe for
       the range expected in the tests (prime(10000) ≈ 104 729).     */
    seq_t p = (seq_t)get_prime(n);

    seq_t p_plus = p + (seq_t)1;

    /* Compute the two terms using the supplied big‑integer power routine. */
    seq_t term1 = big_pow(p, p_plus);   /* p^(p+1)   */
    seq_t term2 = big_pow(p_plus, p);   /* (p+1)^p   */

    return term1 + term2;
}
