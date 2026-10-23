/*
 * A027940  a(n) = T(2*n, n+4), T given by A027935.
 * https://oeis.org/A027940
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+9) - (21420 + 20571*n + 9961*n^2 + 3304*n^3
 *            + 490*n^4 + 364*n^5 - 56*n^6 + 16*n^7)/630 (OEIS A027940, offset
 *            4)
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* offset: values below 4 are undefined */

    /* Fibonacci part: F(2*n + 9) */
    idx_t fn_index = (n << 1) + 9;
    seq_t fib_val = fib(fn_index);

    /* Powers of n */
    seq_t sn   = (seq_t)n;       /* n^1 */
    seq_t n2   = sn * sn;        /* n^2 */
    seq_t n3   = n2 * sn;        /* n^3 */
    seq_t n4   = n3 * sn;        /* n^4 */
    seq_t n5   = n4 * sn;        /* n^5 */
    seq_t n6   = n5 * sn;        /* n^6 */
    seq_t n7   = n6 * sn;        /* n^7 */

    /* Positive terms of the polynomial */
    seq_t pos = 21420
              + 20571 * sn
              + 9961  * n2
              + 3304  * n3
              + 490   * n4
              + 364   * n5
              + 16    * n7;

    /* Negative term of the polynomial */
    seq_t neg = 56 * n6;

    /* Numerator: polynomial value (always non‑negative for n >= 4) */
    seq_t numerator = pos - neg;

    /* Division by 630 */
    seq_t poly_div = numerator / 630;

    /* Final result */
    return fib_val - poly_div;
}
