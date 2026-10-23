/*
 * A165596  Jacobi symbol (n,59881).
 * https://oeis.org/A165596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Jacobi symbol (n,59881)
 * Range:     i8 n <= 9737, i16 n <= 9737, i32 n <= 9737, i64 n <= 9737,
 *            i128 n <= 9737, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    return (seq_t)jacobi_symbol((uint64_t)n, 59881ULL);
}
