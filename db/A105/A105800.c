/*
 * A105800  Greatest Fibonacci number that is a proper divisor of the n-th
 *          Fibonacci number; a(1) = a(2) = 1.
 * https://oeis.org/A105800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A032742(n)) (greatest Fibonacci proper divisor of
 *            F_n)
 * Range:     u8 n <= 27, u16 n <= 49, u32 n <= 95, u64 n <= 187,
 *            u128 n <= 373, big any n
 * Validated: standard, pass, 77 terms through n = 985 (2026-11-01)
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast.h"

/* Greatest proper divisor of n = n / smallest prime factor of n
   (for n == 1 we return 1 by definition of the sequence). */
static inline idx_t largest_proper_divisor(idx_t n)
{
    if (n <= 1) return 1;           /* a(1) = 1 */
    /* Find smallest prime factor. */
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) return n / p;
    }
    /* n is prime */
    return 1;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    idx_t d = largest_proper_divisor(n);   /* d = largest proper divisor of n */

    seq_t fib_d, dummy;
    fib_fast(d, &fib_d, &dummy);           /* fib_fast computes F_d and F_{d+1} */
    return fib_d;                          /* a(n) = F_d */
}
