/*
 * A214962  a(n) is the least m > 0 such that Fibonacci(n-m) divides
 *          Fibonacci(2n+2m).
 * https://oeis.org/A214962
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n - max{d < n : 4n mod d == 0}
   a(n) is the least m > 0 such that Fib(n-m) divides Fib(2n+2m) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 2 */
    if (n < 2)
        return 0;

    /* compute 4*n once (seq_t type to avoid overflow) */
    seq_t four_n = (seq_t)4 * (seq_t)n;

    /* search for the largest divisor d of 4*n that is strictly smaller than n */
    for (idx_t d = n - 1; d >= 1; --d) {
        if (four_n % (seq_t)d == 0) {
            /* corresponding m = n - d */
            return (seq_t)(n - d);
        }
    }

    /* should never reach here for n >= 2 */
    return 0;
}
