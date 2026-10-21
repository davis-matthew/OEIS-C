/*
 * A220115  a(n) = A000120(n) - A007895(n), the number of 1's in binary
 *          expansion of n minus the number of terms in Zeckendorf
 *          representation of n.
 * https://oeis.org/A220115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, easy
 * Formula:   a(n) = A000120(n) - A007895(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Fibonacci numbers used for Zeckendorf representation
       (1, 2, 3, 5, 8, …).  We generate them lazily on first call. */
    static idx_t fibs[93];
    static size_t fib_len = 0;
    if (fib_len == 0) {
        fibs[0] = 1;
        fibs[1] = 2;
        fib_len = 2;
        while (fib_len < 93) {
            idx_t next = fibs[fib_len - 1] + fibs[fib_len - 2];
            if (next < fibs[fib_len - 1])  /* overflow */
                break;
            fibs[fib_len++] = next;
        }
    }

    /* Count 1-bits in n (popcount). */
    seq_t pop = (seq_t)__builtin_popcountll((unsigned long long)n);

    /* Count terms in Zeckendorf representation of n. */
    idx_t   temp = n;
    seq_t   zeck = 0;
    while (temp > 0) {
        /* Find the largest Fibonacci number ≤ temp. */
        idx_t f = 0;
        for (size_t i = fib_len; i > 0; --i) {
            if (fibs[i - 1] <= temp) {
                f = fibs[i - 1];
                break;
            }
        }
        temp -= f;
        zeck++;
    }

    return pop - zeck;
}
