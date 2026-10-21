/*
 * A093045  2*Jacobsthal(n-1)*Fibonacci(n).
 * https://oeis.org/A093045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Jacobsthal(n-1)*Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 by definition */
    if (n <= 1)
        return 0;

    /* Fibonacci(n) */
    seq_t fn = fib(n);

    /* Compute Jacobsthal(n-1) via recurrence:
       J(0) = 0, J(1) = 1, J(k) = J(k-1) + 2*J(k-2)  (k >= 2) */
    idx_t m = n - 1;          /* m >= 1 for n >= 2 */
    seq_t j_prev2 = 0;        /* J(0) */
    seq_t j_prev1 = 1;        /* J(1) */
    seq_t j = 0;              /* J(m) */

    if (m == 1) {
        j = j_prev1; /* J(1) */
    } else {
        for (idx_t k = 2; k <= m; ++k) {
            j = j_prev1 + (j_prev2 << 1); /* 2*J(k-2) + J(k-1) */
            j_prev2 = j_prev1;
            j_prev1 = j;
        }
    }

    /* a(n) = 2 * J(m) * Fib(n) */
    return fn * (j << 1); /* multiply by 2 */
}
