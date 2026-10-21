/*
 * A093122  a(n) = Jacobsthal(n) * Fibonacci(n+1).
 * https://oeis.org/A093122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Jacobsthal(n) * Fibonacci(n+1) Jacobsthal: J(0)=0,
 *            J(1)=1, J(n)=J(n-1)+2*J(n-2) Fibonacci: F(0)=0, F(1)=1,
 *            F(n)=F(n-1)+F(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute Fibonacci(n+1) */
    seq_t fib_prev = 0;   /* F0 */
    seq_t fib_curr = 1;   /* F1 */
    seq_t fib_next;
    if (n >= 1) {
        /* iterate up to n+1 */
        for (idx_t i = 2; i <= n + 1; ++i) {
            fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }
    /* fib_curr now holds F(n+1) */

    /* Compute Jacobsthal(n) */
    seq_t jac_prev = 0;   /* J0 */
    seq_t jac_curr = 1;   /* J1 */
    seq_t jac_next;
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            jac_next = jac_curr + (jac_prev + jac_prev); /* 2*jac_prev */
            jac_prev = jac_curr;
            jac_curr = jac_next;
        }
    }
    /* jac_curr now holds J(n) */

    return jac_curr * fib_curr;
}
