/*
 * A220888  a(n) = F(n+7) - (1/2)*(n^3+2*n^2+13*n+26) where F(i) is a
 *          Fibonacci number (A000045).
 * https://oeis.org/A220888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+7) - (1/2)*(n^3+2*n^2+13*n+26)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn = fib(n + 7);            /* F(n+7)                     */
    seq_t nn = (seq_t)n;              /* cast index to seq_t        */
    seq_t poly = nn * nn * nn;        /* n^3                         */
    poly += (seq_t)2 * nn * nn;       /* + 2*n^2                     */
    poly += (seq_t)13 * nn;           /* + 13*n                      */
    poly += (seq_t)26;                /* + 26                        */
    poly >>= 1;                       /* divide by 2 (exact)         */
    return fn - poly;
}
