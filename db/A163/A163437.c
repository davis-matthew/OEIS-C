/*
 * A163437  Number of different fixed (possibly) disconnected polyominoes (of
 *          any area) bounded tightly by an n X n square.
 * https://oeis.org/A163437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 4*2^((n-1)*n) + 4*2^((n-1)^2) + 2*2^((n-2)*n) -
 *            4*2^((n-2)*(n-1)) + 2^((n-2)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) is defined separately; the formula would underflow for n<2. */
    if (n == 1)
        return (seq_t)1;

    /* For n >= 2 all intermediate expressions are non‑negative. */
    idx_t e1 = n * n;                   /* n^2 */
    idx_t e2 = (n - 1) * n;             /* (n-1)*n */
    idx_t e3 = (n - 1) * (n - 1);       /* (n-1)^2 */
    idx_t e4 = (n - 2) * n;             /* (n-2)*n */
    idx_t e5 = (n - 2) * (n - 1);       /* (n-2)*(n-1) */
    idx_t e6 = (n - 2) * (n - 2);       /* (n-2)^2 */

    seq_t result = pow2(e1);              /* 2^(n^2) */
    result -= pow2(e2 + 2);               /* -4*2^((n-1)*n)  */
    result += pow2(e3 + 2);               /* +4*2^((n-1)^2)  */
    result += pow2(e4 + 1);               /* +2*2^((n-2)*n)  */
    result -= pow2(e5 + 2);               /* -4*2^((n-2)*(n-1)) */
    result += pow2(e6);                   /* +2^((n-2)^2)   */

    return result;
}
