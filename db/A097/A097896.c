/*
 * A097896  Number of compositions of n with either all parts odd or all parts
 *          even.
 * https://oeis.org/A097896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) if n is odd; a(n) = 2^(n/2-1) + Fibonacci(n)
 *            if n is even
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* a(1) through a(24) for reference:
       1,2,2,5,5,12,13,29,34,71,89,176,233,441,610,1115,1597,2840,4181,7277,10946,18735,28657,48416 */

    if (n == 0U) /* undefined, but keep a defined value */
        return 0;

    if ((n & 1U) == 0U)                 /* even n = 2*m */
    {
        idx_t m = n >> 1;               /* m = n/2 */
        seq_t part = pow2(m - 1);       /* 2^(m-1) = 2^(n/2-1) */
        seq_t fibv  = fib(n);           /* Fibonacci(n) */
        return part + fibv;
    }
    else                                /* odd n */
    {
        return fib(n);
    }
}
