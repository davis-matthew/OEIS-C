/*
 * A022389  Fibonacci sequence beginning 7, 15.
 * https://oeis.org/A022389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*Fib(n+2) + Fib(n) (Fib(0)=0, Fib(1)=1,
 *            Fib(n)=Fib(n-1)+Fib(n-2) for n ≥ 2)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f_nm1 = (seq_t)0; /* Fib(0) */
    seq_t f_n   = (seq_t)1; /* Fib(1) */

    /* compute Fib(n) and Fib(n+1) iteratively */
    for (idx_t i = (idx_t)0; i < n; ++i) {
        seq_t next = f_nm1 + f_n;  /* Fib(i+1) */
        f_nm1 = f_n;
        f_n   = next;
    }

    /* now f_nm1 = Fib(n), f_n = Fib(n+1) */
    seq_t f_np2 = f_nm1 + f_n;                     /* Fib(n+2) */
    seq_t result = (seq_t)7 * f_np2 + f_nm1;       /* a(n) */
    return result;
}
