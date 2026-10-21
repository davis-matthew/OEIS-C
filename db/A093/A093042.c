/*
 * A093042  a(n) = Jacobsthal(n) * Fibonacci(n).
 * https://oeis.org/A093042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Jacobsthal(n) * Fibonacci(n) Jacobsthal: J(0)=0, J(1)=1,
 *            J(n)=J(n-1)+2*J(n-2) Fibonacci: F(0)=0, F(1)=1,
 *            F(n)=F(n-1)+F(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The helper function 'fib' is provided by the utils library. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the base case n==0 directly to avoid
       unnecessary work. */
    if (n == 0)
        return 0;

    /* Compute Fibonacci(n) using the provided helper. */
    seq_t fib_n = fib(n);

    /* Compute Jacobsthal(n) iteratively. */
    seq_t jac0 = 0;        /* J(0) */
    seq_t jac1 = 1;        /* J(1) */

    if (n == 1)
    {
        return jac1 * fib_n;
    }

    seq_t jac_next;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* J(i) = J(i-1) + 2*J(i-2) */
        jac_next = jac1 + (jac0 << 1);  /* multiply by 2 via shift */
        jac0 = jac1;
        jac1 = jac_next;
    }

    /* Result is J(n) * F(n) */
    return jac1 * fib_n;
}
