/*
 * A238244  A recursive sequence: a(n) = Fibonacci(n)*a(n-1) + 3.
 * https://oeis.org/A238244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=Fib(n)*a(n-1)+3 (n≥2; Fib(1)=1, Fib(2)=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t a_val = (seq_t)1;          /* a(1) */
    seq_t fib_prevprev = (seq_t)1;   /* Fib(1) */
    seq_t fib_prev = (seq_t)1;       /* Fib(2) */
    seq_t fib;                       /* current Fib(i) */

    /* compute a(2) */
    a_val = fib_prev * a_val + (seq_t)3;
    if (n == 2) return a_val;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Fib(i) = Fib(i-1) + Fib(i-2) */
        fib = fib_prevprev + fib_prev;
        fib_prevprev = fib_prev;
        fib_prev = fib;

        a_val = fib * a_val + (seq_t)3;
    }

    return a_val;
}
