/*
 * A097541  Sum_{k=1..n-1} J(2*n,k)*k, where J(i,j) is the Jacobi symbol.
 * https://oeis.org/A097541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n-1} J(2*n,k) * k, where J(i,j) is the Jacobi
 *            symbol
 * Range:     i8 n <= 31, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   jacobi_symbol_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol_uint64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n <= 1) return 0;              /* a(1) = 0 */
    uint64_t two_n = (uint64_t)n * 2ULL;
    for (idx_t k = 1; k < n; ++k) {
        int js = jacobi_symbol_uint64(two_n, (uint64_t)k);
        if (js != 0)
            sum += (seq_t)k * js;
    }
    return sum;
}
