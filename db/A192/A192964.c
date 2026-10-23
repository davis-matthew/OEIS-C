/*
 * A192964  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0)=1, a(1)=0, a(2)=3, a(3)=7
   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4)   (n≥4)
   (implemented via the equivalent formula
    a(n) = Fibonacci(n+3)+3*Fibonacci(n+1) - 2*(n+2).) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Direct return for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    /* Compute the needed Fibonacci numbers: Fib(n+1) and Fib(n+3) */
    idx_t target_n1 = n + 1;
    idx_t target_n3 = n + 3;

    seq_t fib_prev2 = (seq_t)0;   /* Fib(i-2)  */
    seq_t fib_prev1 = (seq_t)1;   /* Fib(i-1)  */
    seq_t fib_curr  = 0;          /* Fib(i)    */

    seq_t fib_n1 = 0;
    seq_t fib_n3 = 0;

    for (idx_t i = 2; i <= target_n3; ++i) {
        fib_curr = fib_prev2 + fib_prev1;   /* Fib(i) = Fib(i-2)+Fib(i-1) */
        fib_prev2 = fib_prev1;
        fib_prev1 = fib_curr;

        if (i == target_n1) fib_n1 = fib_curr;
        if (i == target_n3) fib_n3 = fib_curr;
    }

    /* a(n) = Fib(n+3) + 3*Fib(n+1) - 2*(n+2) */
    seq_t three_fib_n1 = fib_n1 + fib_n1 + fib_n1;          /* 3 * Fib(n+1) */
    seq_t sum = fib_n3 + three_fib_n1;                     /* Fib(n+3) + 3*Fib(n+1) */
    seq_t sub  = ((seq_t)2) * ((seq_t)n + (seq_t)2);       /* 2*(n+2)  */
    seq_t result = sum - sub;

    return result;
}
