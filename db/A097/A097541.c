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
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Jacobi symbol (a|n) for odd n.  For even n the result is 0. */
static int jacobi_symbol_uint64(uint64_t a, uint64_t n)
{
    if (n == 0) return 0;
    if ((n & 1) == 0) return 0;           /* denominator must be odd */
    a %= n;
    int result = 1;
    while (a != 0) {
        while ((a & 1) == 0) {            /* a is even */
            a >>= 1;
            uint64_t n_mod_8 = n & 7;
            if (n_mod_8 == 3 || n_mod_8 == 5)
                result = -result;
        }
        /* swap a and n */
        uint64_t temp = a;
        a = n;
        n = temp;
        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;
        a %= n;
    }
    return (int)(n == 1 ? result : 0);
}

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
