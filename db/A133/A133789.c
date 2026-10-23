/*
 * A133789  Let P(A) denote the power set of an n-element set A. Then a(n) =
 *          the number of pairs of elements {x,y} of P(A) for which either 0)
 *          x and y are disjoint and for which x is not a subset of y and y is
 *          not a subset of x, 1) x and y are disjoint and for which either x
 *          is a subset of y or y is a subset...
 * https://oeis.org/A133789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)(4^n - 2*3^n + 3*2^n - 2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    // Compute the needed powers
    seq_t four_pow = pow4(n);  // 4^n
    seq_t three_pow = pow3(n); // 3^n
    seq_t two_pow = pow2(n);   // 2^n

    // We evaluate the expression in an order that guarantees no
    // underflow for unsigned seq_t: 4^n + 3*2^n - 2*3^n - 2
    seq_t result = four_pow + (3 * two_pow);   // 4^n + 3*2^n
    result -= (2 * three_pow);                 // subtract 2*3^n
    result -= 2;                               // subtract the constant 2

    // Final division by 2 (the result is always even)
    return result / 2;
}
