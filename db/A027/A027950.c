/*
 * A027950  a(n) = T(2n,n+2), T given by A027948.
 * https://oeis.org/A027950
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(2*n + 6) - (48 + 47*n + 23*n^2 + 4*n^3 + 4*n^4)/6 (n
 *            ≥ 3) a(2) = 1 (offset = 2) (All computations are performed in
 *            the type seq_t)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* undefined behavior below the offset is acceptable */
    if (n < 2)
        return (seq_t)0;

    if (n == 2)
        return (seq_t)1;

    /* Fibonacci part – uses the utility function fib(idx_t) */
    idx_t m = 2*n + 6;
    seq_t f = fib(m);

    /* Polynomial part: (48 + 47*n + 23*n^2 + 4*n^3 + 4*n^4) / 6 */
    seq_t sn   = (seq_t)n;
    seq_t num  = (seq_t)48
                + (seq_t)47 * sn
                + (seq_t)23 * sn * sn
                + (seq_t)4  * sn * sn * sn
                + (seq_t)4  * sn * sn * sn * sn;
    seq_t poly = num / 6;

    return f - poly;
}
