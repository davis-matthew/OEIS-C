/*
 * A011593  Legendre symbol (n,53).
 * https://oeis.org/A011593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) == n^26 (mod 53) (Legendre symbol (n,53))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* The Jacobi symbol coincides with the Legendre symbol for an odd prime, 53. */
    return (seq_t)jacobi_symbol(n, 53);
}
