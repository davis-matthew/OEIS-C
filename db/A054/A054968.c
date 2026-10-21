/*
 * A054968  3*Fibonacci(n) - 11.
 * https://oeis.org/A054968
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fibonacci(n) - 11, offset 5 (a(5) = 4)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Fibonacci(n) iteratively, then applies the formula. */

#include <stddef.h>
#include <stdint.h>

/* Indices start from 0.  For n < 5 the function returns 0
 * (behaviour for undefined indices is irrelevant). */
seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;
    }

    /* Fibonacci initialization: F(0)=0, F(1)=1 */
    seq_t f0 = 0;  /* F(i-2) */
    seq_t f1 = 1;  /* F(i-1) */
    seq_t fn;      /* current Fibonacci value */

    /* Handle the trivial cases directly. */
    if (n == 0) {
        fn = f0;
    } else if (n == 1) {
        fn = f1;
    } else {
        /* Iterate up to n computing Fibonacci numbers. */
        for (idx_t i = 2; i <= n; ++i) {
            fn = f0 + f1;
            f0 = f1;
            f1 = fn;
        }
    }

    /* Compute a(n) = 3*Fibonacci(n) - 11 */
    seq_t res = fn * (seq_t)3 - (seq_t)11;
    return res;
}
