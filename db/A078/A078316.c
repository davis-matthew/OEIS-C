/*
 * A078316  Maximum exponent in the prime factorization of n*rad(n)+1, where
 *          rad = A007947 (squarefree kernel).
 * https://oeis.org/A078316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum exponent in the prime factorization of
 *            n*rad(n)+1, where rad(n) = product of distinct primes dividing
 *            n. The function uses simple trial division and works for all n
 *            >= 1 that fit within the limits of a 64‑bit integer. For larger
 *            builds the seq_t type is assumed to hold arbitrary pr...
 * Range:     u8 n <= 22, u16 n <= 847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_rad, factor_max_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/calc_rad.h"
#include "../utils/factor_max_exponent.h"

seq_t a(idx_t n)
{
    /* Rad(n) */
    seq_t rad = calc_rad(n);
    /* Compute m = n*rad + 1  (use seq_t for potential overflow) */
    seq_t m = (seq_t)n * rad + 1;
    /* Find maximum exponent in factorization of m */
    return factor_max_exponent(m);
}
