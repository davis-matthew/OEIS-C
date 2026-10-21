/*
 * A206605  Fibonacci sequence beginning 14, 11.
 * https://oeis.org/A206605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+5) + Fibonacci(n) + Fibonacci(n-6)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_of.h"

seq_t a(idx_t n)
{
    seq_t fn, fn5;
    fib_of(n, &fn);
    fib_of(n + 5, &fn5);
    seq_t sum = fn + fn5;

    if (n < 6) {
        /* Handle F(n-6) where n-6 < 0 :  F(-k) = (-1)^(k+1) * F(k)  */
        idx_t k = 6 - n;          /* 1 <= k <= 5 */
        seq_t fk;
        fib_of(k, &fk);
        if ((k & 1) == 0)         /* k even → sign −1 */
            sum -= fk;
        else                      /* k odd  → sign +1 */
            sum += fk;
    } else {
        seq_t fn6;
        fib_of(n - 6, &fn6);
        sum += fn6;
    }

    return sum;
}
