/*
 * A022088  Fibonacci sequence beginning 0, 5.
 * https://oeis.org/A022088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * Fib(n), Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the trivial case first */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute Fib(n) iteratively using two accumulator variables.
       This works for any seq_t that supports +, and for arbitrary-size
       integers. */
    seq_t f0 = (seq_t)0;      /* Fib(0) */
    seq_t f1 = (seq_t)1;      /* Fib(1) */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        seq_t tmp = f0 + f1;  /* Fib(i+1) = Fib(i-1)+Fib(i) */
        f0 = f1;
        f1 = tmp;
    }

    /* Multiply by 5 to obtain a(n) */
    return f1 * (seq_t)5;
}
