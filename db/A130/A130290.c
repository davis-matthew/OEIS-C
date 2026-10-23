/*
 * A130290  Number of nonzero quadratic residues modulo the n-th prime.
 * https://oeis.org/A130290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( get_prime(n) / 2 )
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the number of non‑zero quadratic residues modulo the n‑th prime */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get_prime(1) == 2, get_prime(2) == 3, ... */
    uint32_t p = get_prime(n);
    /* integer division truncates toward zero, which is floor(...) for positive values */
    return ((seq_t)p) / 2;
}
