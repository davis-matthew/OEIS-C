/*
 * A020550  4th Fibonacci polynomial evaluated at x=n!.
 * https://oeis.org/A020550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^3 + 2*n!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);          /* f = n!   */
    seq_t res = f * f;               /* f^2     */
    res = res * f;                   /* f^3     */
    res = res + (f << 1);            /* f^3 + 2f*/
    return res;
}
