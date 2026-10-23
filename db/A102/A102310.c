/*
 * A102310  Square array read by antidiagonals: Fibonacci(k*n).
 * https://oeis.org/A102310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci( k * (d - k + 1) ), where n = d(d-1)/2 + k, 1 ≤
 *            k ≤ d, d minimal such that n ≤ d(d+1)/2
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 83, u64 n <= 178,
 *            u128 n <= 361, big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    /* Find d such that n <= d(d+1)/2 */
    idx_t d = 1;
    while (n > d * (d + 1) / 2)
        ++d;

    idx_t prev = (d - 1) * d / 2;
    idx_t k = n - prev;                 /* 1 .. d */
    idx_t prod = k * (d - k + 1);        /* k * (d - k + 1) */

    seq_t f, f_next;
    fib_fast(prod, &f, &f_next);        /* f = F(prod)   */
    return f;
}
