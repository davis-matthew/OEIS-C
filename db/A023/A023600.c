/*
 * A023600  Convolution of A023532 and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib(n+3) - 2 - Σ_{k≥1, k(k+3)/2 ≤ n} fib(n - k(k+3)/2 +
 *            2)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial value: sum of all Fibonacci numbers from F(2) to F(n+1) */
    seq_t result = fib(n + 3);      /* F(n+3) */
    result -= (seq_t)2;             /* subtract 2  (F(0)+F(1)) */

    /* subtract contributions of the indices where A023532 has a 0.
       These indices are m = k(k+3)/2, k=1,2,3,... */
    for (idx_t k = 1; ; ++k) {
        idx_t m = k * (k + 3) / 2;   /* m ≤ n ? */
        if (m > n)
            break;
        /* the corresponding Fibonacci term in the convolution */
        result -= fib(n - m + 2);
    }
    return result;
}
