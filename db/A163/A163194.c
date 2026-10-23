/*
 * A163194  a(n) = F(n)^2 * L(n+1)^2 * F(n-1) * L(n+2), where F(n) and L(n)
 *          are the Fibonacci and Lucas numbers, respectively.
 * https://oeis.org/A163194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2n+1)^3 - 3*F(2n+1) + 2*(-1)^n where F is the Fibonacci
 *            sequence
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 0 by definition. */
    if (n < 2)
        return 0;

    /* Compute F(2n+1). */
    idx_t k = 2 * n + 1;
    seq_t f = fib(k);

    /* Compute f^3 and 3*f. */
    seq_t f2 = f * f;          /* F^2 */
    seq_t f3 = f2 * f;         /* F^3 */
    seq_t three_f = f * 3;     /* 3*F */

    /* term = f^3 - 3*f */
    seq_t term = f3 - three_f;

    /* Add or subtract 2 depending on (-1)^n. */
    if ((n & 1) == 0)          /* n even -> (-1)^n = 1 */
        term += 2;
    else                       /* n odd -> (-1)^n = -1 */
        term -= 2;

    return term;
}
