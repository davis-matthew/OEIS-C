/*
 * A023896  Sum of positive integers in smallest positive reduced residue
 *          system modulo n. a(1) = 1 by convention.
 * https://oeis.org/A023896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n * phi(n) / 2 for n > 1; a(1) = 1 where phi(n) is
 *            Euler's totient function
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi_from_idx.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t phi = phi_from_idx(n);
    return (seq_t)n * phi / 2;
}
