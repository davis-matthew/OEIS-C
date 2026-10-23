/*
 * A135172  a(n) = 3^prime(n) + 2^prime(n).
 * https://oeis.org/A135172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^prime(n) + 2^prime(n) (OEIS A135172)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – a(1) is the first term.  If n == 0,
       the sequence is undefined; we return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Get the n‑th prime (1‑based).  get_prime returns a uint32_t,
       which easily fits into idx_t; cast explicitly for clarity. */
    idx_t p = (idx_t)get_prime(n);

    /* Compute 3^p and 2^p using the arbitrary‑precision routine
       pow_uint( base, exponent ).  Then add the two results. */
    seq_t pow3 = pow_uint(3, p);
    seq_t pow2 = pow_uint(2, p);
    return pow3 + pow2;
}
