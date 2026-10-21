/*
 * A081075  a(n) = Fibonacci(4n+2) - 3.
 * https://oeis.org/A081075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(4*n + 2) - 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here Fib(0)=0, Fib(1)=1.  n=1 corresponds to the first term 5. */

seq_t a(idx_t n)
{
    /* Index of the Fibonacci number required */
    idx_t k = (idx_t)4 * n + (idx_t)2;

    /* Compute Fib(k) iteratively using only addition */
    seq_t f0 = (seq_t)0;    /* Fib(0) */
    seq_t f1 = (seq_t)1;    /* Fib(1) */
    idx_t i = (idx_t)0;

    while (i < k)
    {
        seq_t tmp = f0 + f1;  /* Fib(i+1) */
        f0 = f1;              /* Fib(i+1) becomes Fib(i) for next step */
        f1 = tmp;             /* Fib(i+2) becomes Fib(i+1) */
        ++i;
    }

    /* f0 now equals Fib(k); subtract 3 to get the sequence value */
    return f0 - (seq_t)3;
}
