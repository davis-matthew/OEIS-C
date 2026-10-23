/*
 * A126996  a(n) = binomial(prime(3+n), prime(3)).
 * https://oeis.org/A126996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(prime(3+n), prime(3))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* prime(3) is the constant 5 */
    const idx_t p_small = 5;
    /* prime(3+n) */
    const idx_t p_big   = get_prime(n + 3);
    return binomial(p_big, p_small);
}
