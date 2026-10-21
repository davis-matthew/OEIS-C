/*
 * A078314  Total number of prime factors of n*rad(n)+1 counted with
 *          multiplicity, where rad = A007947 (squarefree kernel).
 * https://oeis.org/A078314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(n*rad(n)+1) (total number of prime factors counted with
 *            multiplicity)
 * Range:     u8 n <= 16, u16 n <= 1595, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Compute the square‑free part of n */
    seq_t rad_n = rad(n);

    /* Compute n * rad(n) + 1, taking care to promote n to the correct type */
    seq_t val = ((seq_t)n * rad_n) + 1;

    /* Count prime factors of val, counting multiplicities */
    return count_prime_factors(val);
}
