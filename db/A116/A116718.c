/*
 * A116718  Number of permutations of length n which avoid the patterns 321,
 *          1342, 3124.
 * https://oeis.org/A116718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+3) + max(0, 2*n-9) − max(0, 9-2*n) (n>2)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* n >= 3 : use a(n) = F(n+3) + 2*n - 9  (F = Fibonacci)   */
    seq_t res = fib(n + 3);          /* F(n+3) */
    if (n >= 5)                      /* 2*n-9 >= 0   */
    {
        res += (seq_t)((seq_t)2 * n - (seq_t)9);
    }
    else /* n == 3 or n == 4 : 2*n-9 < 0, subtract |2*n-9| */
    {
        seq_t offset = (seq_t)((seq_t)9 - (seq_t)2 * n); /* positive value */
        res -= offset;
    }
    return res;
}
