/*
 * A023864  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2) and t = (F(2), F(3), F(4), ...), F(n) =
 *          Fibonacci(n).
 * https://oeis.org/A023864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{i=1}^{\lfloor (n+1)/2 \rfloor} i * F(n+2-i) where
 *            F(m) is the mth Fibonacci number (F(0)=0,F(1)=1)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                 /* for uint64_t used in idx_t */
#include "../utils/fib.h"           /* provides seq_t fib(idx_t n) */

/* Compute the nth term of OEIS sequence A023864.  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n<1, return 0 */

    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) >> 1;

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t multiplier = (seq_t)i;
        seq_t fibval     = fib(n + 2 - i);   /* F(n+2-i) */
        sum += multiplier * fibval;
    }
    return sum;
}
