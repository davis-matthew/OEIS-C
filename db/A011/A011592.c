/*
 * A011592  Legendre symbol (n,47).
 * https://oeis.org/A011592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n|47)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Reduce the index modulo 47 */
    uint64_t r = n % 47ULL;
    /* Compute the Jacobi (Legendre) symbol (r|47) */
    int val = jacobi_symbol(r, 47ULL);
    /* Return the result as seq_t (may be int8, int64, big integer, etc.) */
    return (seq_t)val;
}
