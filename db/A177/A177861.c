/*
 * A177861  Product of the quadratic nonresidues of prime(n).
 * https://oeis.org/A177861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of the quadratic nonresidues of the nth prime
 *            (product over 1..p−1 of k such that k is a quadratic nonresidue
 *            modulo p, where p = prime(n)). This implementation uses Legendre
 *            symbols (via jacobi_symbol for odd primes) and a simple loop
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* p is the nth prime; for n==1 p==2 where there are no nonresidues,
       so the product is defined to be 1. */
    uint64_t p = get_prime(n);
    seq_t prod = (seq_t)1;

    for (uint64_t k = 1; k < p; ++k) {
        /* jacobi_symbol returns +1 for residues, -1 for nonresidues,
           and 0 if k divisible by p (never here). */
        if (jacobi_symbol(k, p) == -1) {
            prod *= (seq_t)k;
        }
    }
    return prod;
}
