/*
 * A026822  CONTINUANT transform of Fibonacci number 1, 1, 2, 3, 5, 8, ...
 * https://oeis.org/A026822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 2 for n ≥ 3 : a(n) = fib(n) * a(n‑1) + a(n‑2)
 *            where fib(1)=1, fib(2)=1, fib(k)=fib(k-1)+fib(k-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial cases directly */
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = 1;   /* a(1) */
    seq_t a_nm1 = 2;   /* a(2) */

    /* Fibonacci numbers: fib_prev = F(n-2), fib_curr = F(n-1) */
    seq_t fib_prev = 1;   /* F(1) */
    seq_t fib_curr = 1;   /* F(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Advance Fibonacci sequence */
        seq_t fib_next = fib_prev + fib_curr; /* F(i) */
        fib_prev = fib_curr;
        fib_curr = fib_next;

        /* Continuant recurrence */
        seq_t a_i = fib_curr * a_nm1 + a_nm2;

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
