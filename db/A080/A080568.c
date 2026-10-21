/*
 * A080568  Sum of the Fibonacci numbers A000045 and the factorials A000142.
 * https://oeis.org/A080568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fib(n) + n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f;
    /* compute Fibonacci F(n) */
    if (n == 0) {
        f = 0;
    } else if (n == 1) {
        f = 1;
    } else {
        seq_t f0 = 0, f1 = 1, tmp = 0;
        for (idx_t k = 2; k <= n; ++k) {
            tmp = f1;
            f1 = f0 + f1;
            f0 = tmp;
        }
        f = f1;
    }

    /* compute factorial n! */
    seq_t fact = 1;
    for (idx_t k = 2; k <= n; ++k) {
        fact = fact * (seq_t)k;
    }

    return f + fact;
}
