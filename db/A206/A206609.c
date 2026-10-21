/*
 * A206609  Fibonacci sequence beginning 13, 9.
 * https://oeis.org/A206609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2); a(1) = 13, a(2) = 9 Also a(n) =
 *            13*Fib(n) - 4*Fib(n-1) where Fib(0)=0, Fib(1)=1 Computes a(n)
 *            for any n ≥ 1 (behaviour for n < 1 is undefined)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0) return 0;          /* undefined for offset 1, safe fallback */
    if (n == 1) return 13;
    if (n == 2) return 9;

    /* Compute Fibonacci numbers Fib(n) and Fib(n-1). */
    seq_t f_prev = 0;   /* Fib(0) */
    seq_t f_cur  = 1;   /* Fib(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = tmp;
    }

    /* a(n) = 13*Fib(n) - 4*Fib(n-1) */
    seq_t result = (seq_t)13 * f_cur;
    result -= (seq_t)4 * f_prev;
    return result;
}
