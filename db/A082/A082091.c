/*
 * A082091  a(n) = one more than the number of iterations of A005361 needed to
 *          reach 1 from the starting value n.
 * https://oeis.org/A082091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(A005361(n)), a(1) = 1 A005361(n) = product of the
 *            exponents in the prime factorisation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prod_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prod_exp.h"

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t steps = 0;

    while (x != 1) {
        seq_t f = prod_exp((uint64_t)x);
        x = (idx_t)f;   /* f will fit in idx_t for the intended ranges */
        ++steps;
    }

    return steps + 1;
}
