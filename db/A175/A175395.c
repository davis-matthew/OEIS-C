/*
 * A175395  a(n) = 2*Fibonacci(n)^2.
 * https://oeis.org/A175395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fib(n)^2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th Fibonacci number (Fib(0)=0, Fib(1)=1) using only
 * addition. The result is then squared and multiplied by 2 to obtain
 * the term of sequence A175395.
 */
seq_t a(idx_t n)
{
    seq_t f_prev = (seq_t)0;   /* Fib(0) */
    seq_t f_curr = (seq_t)1;   /* Fib(1) */

    /* Special case n = 0 */
    if (n == 0)
        return f_prev;        /* 2*0^2 = 0 */

    /* Iteratively compute Fib(n). */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t tmp = f_prev + f_curr; /* next Fibonacci number */
        f_prev = f_curr;
        f_curr = tmp;
    }

    seq_t f = f_prev;          /* Fib(n) is now in f_prev */
    seq_t res = f * f;         /* f^2      */
    res = res * ((seq_t)2);    /* 2*f^2    */
    return res;
}
