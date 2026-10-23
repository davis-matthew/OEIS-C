/*
 * A123231  Row sums of A123230.
 * https://oeis.org/A123231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k-1) = Fibonacci(k), a(2k) = Fibonacci(k+2)
 * Range:     u8 n <= 23, u16 n <= 45, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 369, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based.  For odd n: k = (n+1)/2, a(n)=fib(k).         */
/* For even n: k = (n+1)/2, a(n)=fib(k+2).                     */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset guard, undefined otherwise
    /* compute k = floor((n+1)/2) which works for both parities */
    idx_t k = (n + 1) / 2;
    /* add 2 only if n is even */
    if ((n & 1ULL) == 0ULL)
        k += 2;
    return fib(k);
}
